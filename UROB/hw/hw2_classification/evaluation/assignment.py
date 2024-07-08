from dataclasses import dataclass


@dataclass
class Assignment:
    name: str
    test_func: callable
    verification_file: str

    def test_assignment(self, src_dir: str, generate: bool, seed: int):
        kwargs = dict(
            seed=seed,
            src_dir=src_dir,
            generate=generate,
            verification_file=self.verification_file
        )
        results = self.test_func(**kwargs)
        return results
