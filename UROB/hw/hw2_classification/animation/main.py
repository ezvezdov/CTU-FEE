import os
import sys
import pickle
import shutil
import argparse

import yaml
from manim import *
from tqdm import tqdm
from PIL import Image
from jinja2 import Template

ROOT_PATH = os.path.join(os.path.dirname(os.path.realpath(__file__)), "..")
sys.path.append(ROOT_PATH)

from assignments.mlp_classifier import MLPClassifier


def load_config(config_path: str, args: dict) -> dict:
    with open(config_path) as file:
        template = Template(file.read())
    cfg_text = template.render(**args)
    cfg = yaml.load(cfg_text, Loader=yaml.FullLoader)
    return cfg


def get_points(dataset_path: str, colors: dict, dot_config: dict, transformed: bool = False,
               model: MLPClassifier = None):
    dataset = np.load(dataset_path)
    X, y = dataset['X_test'], dataset['y_test']

    if transformed:
        assert model is not None, 'Model must be provided if transformed is True'
        X = model.transform(X)

    points = [Dot([x[0], x[1], 0], color=colors[int(y[i])], **dot_config) for i, x in enumerate(X)]
    return VGroup(*points)


def get_predictions(model: MLPClassifier, colors: dict, square_config: dict, transformed: bool = False):
    x_range = np.arange(-5, 5, 0.05)
    y_range = np.arange(-5, 5, 0.05)
    X, Y = np.meshgrid(x_range, y_range)
    coords = np.vstack([X.ravel(), Y.ravel()]).transpose()

    if transformed:
        y_pred = model.predict_transformed(coords)
    else:
        y_pred = model.predict(coords)

    squares = []
    for x, pred in tqdm(zip(coords, y_pred), desc='Creating squares to visualize predictions'):
        square = Square(side_length=0.075, color=colors[int(pred)], **square_config)
        square.move_to([x[0], x[1], 0])
        squares.append(square)
    return VGroup(*squares)


def get_model(model_path: str):
    with open(model_path, 'rb') as file:
        model = pickle.load(file)
    return model


def get_frame_properties(points: VGroup):
    max_x = max(dot.get_x() for dot in points)
    min_x = min(dot.get_x() for dot in points)
    max_y = max(dot.get_y() for dot in points)
    min_y = min(dot.get_y() for dot in points)

    center = (max_x + min_x) / 2, (max_y + min_y) / 2, 0
    width, height = max_x - min_x + 5, max_y - min_y + 5

    return center, width, height


def generate_decision_boundaries(model, colors, camera, resolution=(1920, 1080), transformed=False):
    x, y, _ = camera.frame_center
    w, h = camera.frame_width, camera.frame_height

    x_range = np.linspace(x - w / 2, x + w / 2, resolution[0])
    y_range = np.linspace(y - h / 2, y + h / 2, resolution[1])
    X, Y = np.meshgrid(x_range, y_range)
    coords = np.vstack([X.ravel(), Y.ravel()]).transpose()

    if transformed:
        y_pred = model.predict_transformed(coords)
    else:
        y_pred = model.predict(coords)

    color_pred = np.concatenate([np.array(colors[int(pred)]) for pred in y_pred]).astype(np.uint8)
    img = color_pred.reshape(resolution[1], resolution[0], 3)
    img = np.flip(img, 0)

    # Create ImageMobject
    img = Image.fromarray(img)
    img.save("decision_boundaries.png")
    img_mobject = ImageMobject("decision_boundaries.png")
    os.remove("decision_boundaries.png")

    # Set properties
    img_mobject.move_to(camera.frame_center)
    img_mobject.height, img_mobject.width = h, w
    img_mobject.z_index = -1
    return img_mobject


def scale_and_move_text(text: Text, camera: Camera):
    scale_text(text, camera)
    move_to_corner(text, camera)


def move_to_corner(text: Text, camera: Camera):
    UL_corner = camera.frame.get_corner(UL)
    offset = text.height * DOWN * 2 + text.width * RIGHT * 0.55
    text.move_to(UL_corner + offset)


def scale_text(text: Text, camera: Camera):
    width_scale = camera.frame_width / config["frame_width"]
    height_scale = camera.frame_height / config["frame_height"]

    # Determine the minimum scale factor
    min_scale = min(width_scale, height_scale)
    text.font_size *= min_scale


class NNTransform(MovingCameraScene):
    def construct(self):
        # Planes
        foreground_plane = NumberPlane(**cfg["foreground_plane"])
        background_plane = NumberPlane(**cfg["background_plane"])

        # Points
        model = get_model(cfg["model_path"])
        points = get_points(cfg["dataset_path"], cfg["colors"], cfg["dots"])
        transformed_points = get_points(cfg["dataset_path"], cfg["colors"],
                                        cfg["dots"], transformed=True, model=model)

        # Text
        title = Text("Feature Space Transformation", font_size=64, z_index=10)
        orig_coords_text = Text("Dataset in original coordinates", font_size=32, z_index=10)
        scale_and_move_text(orig_coords_text, self.camera)
        orig_prediction_text = Text("Predictions in original coordinates", font_size=32, z_index=10, color=BLACK)
        scale_and_move_text(orig_prediction_text, self.camera)

        # Initialize camera
        self.wait()

        # =========== Intro ===========
        self.play(Write(title))
        self.wait(2)
        self.play(FadeOut(title))
        self.wait()

        # =========== Original coordinates ===========
        self.play(Write(orig_coords_text), FadeIn(background_plane, run_time=2))
        self.play(Create(foreground_plane, run_time=2))
        self.wait()

        self.play(Create(points, run_time=2))
        self.wait(2)

        # =========== Original predictions ===========
        decision_boundaries = generate_decision_boundaries(model, cfg["light_colors_rgb"],
                                                           self.camera, transformed=False)

        self.play(FadeIn(decision_boundaries, run_time=2),
                  Transform(orig_coords_text, orig_prediction_text))
        self.wait(2)

        self.play(FadeOut(decision_boundaries, run_time=2),
                  FadeOut(orig_coords_text, run_time=2))
        self.wait()

        # =========== Transformed coordinates ===========
        center, width, height = get_frame_properties(transformed_points)
        foreground_plane.prepare_for_nonlinear_transform()

        self.play(foreground_plane.animate.apply_function(model.transform_point),
                  self.camera.frame.animate.set_width(width).set_height(height).move_to(center),
                  Transform(points, transformed_points, path_func=utils.paths.straight_path()),
                  run_time=5)
        self.wait(2)

        # Transformed text
        trans_coords_text = Text("Dataset in transformed coordinates", font_size=32, z_index=10)
        scale_text(trans_coords_text, self.camera)
        move_to_corner(trans_coords_text, self.camera)
        trans_prediction_text = Text("Predictions in transformed coordinates", font_size=32, z_index=10,
                                     color=BLACK)
        scale_text(trans_prediction_text, self.camera)
        move_to_corner(trans_prediction_text, self.camera)

        decision_boundaries = generate_decision_boundaries(model, cfg["light_colors_rgb"],
                                                           self.camera, transformed=True)

        self.play(FadeIn(trans_coords_text, run_time=2))
        self.wait(2)

        # =========== Transformed predictions ===========
        self.play(FadeIn(decision_boundaries, run_time=2),
                  Transform(trans_coords_text, trans_prediction_text))
        self.wait(2)

        self.play(FadeOut(decision_boundaries),
                  FadeOut(trans_prediction_text),
                  FadeOut(points),
                  FadeOut(foreground_plane), run_time=2)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="Create an animation of a neural network transformation.")
    parser.add_argument("--dataset", "-d", type=str, default="circles",
                        help="Dataset and corresponding model to use.",
                        choices=["circles", "linearly_separable", "moons", "spirals"])
    parser.add_argument("--activation_function", "-a", type=str, default="relu",
                        help="Activation function to use.",
                        choices=["relu", "tanh", "sigmoid"])
    args = parser.parse_args()
    args.root_path = ROOT_PATH

    config_path = os.path.join(ROOT_PATH, "animation", "config.yaml")
    cfg = load_config(config_path, vars(args))

    for name, value in cfg["manim"].items():
        config[name] = value

    scene = NNTransform()
    scene.render()

    media_dir = os.path.join(ROOT_PATH, "media")

    # Remove media directory if it already exists
    if os.path.isdir(media_dir):
        shutil.rmtree(media_dir)
