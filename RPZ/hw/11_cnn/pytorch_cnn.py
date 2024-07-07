import torch
import torch.nn as nn
import torch.nn.functional as F

class FCNet(nn.Module):
    def __init__(self):
        super(FCNet, self).__init__()

        self.fc = nn.Linear(in_features=28 * 28, out_features=1000)
        self.fc2 = nn.Linear(in_features=1000, out_features=2000)
        self.fc3 = nn.Linear(in_features=2000, out_features=10)

    def forward(self, x):
        x = torch.flatten(x, start_dim=1)
        x = self.fc(x)
        x = torch.sigmoid(x)
        x = self.fc2(x)
        x = torch.relu(x)
        x = self.fc3(x)
        output = F.log_softmax(x, dim=1)
        return output


class SimpleCNN(nn.Module):
    def __init__(self):
        super(SimpleCNN, self).__init__()
        self.conv = nn.Conv2d(in_channels=1,
                              out_channels=10,
                              kernel_size=3,
                              stride=2,
                              padding=1)
        self.fc = nn.Linear(in_features=28 * 28 * 10 // (2 * 2),
                            out_features=10)

    def forward(self, x):
        x = self.conv(x)
        x = F.relu(x)
        x = torch.flatten(x, start_dim=1)
        x = self.fc(x)
        output = F.log_softmax(x, dim=1)
        return output

class MyNet(nn.Module):
    def __init__(self):
        super(MyNet, self).__init__()
        self.conv1 = nn.Conv2d(in_channels=1, out_channels=32, kernel_size=7, padding=1)

        self.conv2 = nn.Conv2d(in_channels=32, out_channels=32, kernel_size=3)

        self.conv3 = nn.Conv2d(in_channels=32, out_channels=64, kernel_size=3)

        # self.conv4 = nn.Conv2d(in_channels=64, out_channels=64, kernel_size=3)

        self.fc = nn.Linear(in_features=5184,
                            out_features=10)

    def forward(self, x):
        x = self.conv1(x)
        x = F.relu(x)

        x = self.conv2(x)
        x = F.relu(x)
        x = F.max_pool2d(x,2)

        x = self.conv3(x)
        x = F.relu(x)

        # x = self.conv4(x)
        # x = F.relu(x)
        # x = F.max_pool2d(x,2)

        x = torch.flatten(x, start_dim=1)
        x = self.fc(x)
        output = F.log_softmax(x, dim=1)
        return output




# class MyNet(nn.Module):
#     """
#     Experiment with all possible settings mentioned in the CW page
#     """
#     def __init__(self):
#         super(MyNet, self).__init__()

#         self.conv1 = nn.Conv2d(in_channels=1, out_channels=2, kernel_size=3)

#         self.conv2 = nn.Conv2d(in_channels=2, out_channels=4, kernel_size=3)

#         self.fc = nn.Linear(in_features=2304, out_features=10)

#     def forward(self, x):
#         x = self.conv1(x)
#         x = F.relu(x)

#         x = self.conv2(x)
#         F.max_pool2d(x, 2)
#         x = F.relu(x)
        
#         x = torch.flatten(x, start_dim=1)
#         x = self.fc(x)
#         output = F.log_softmax(x, dim=1)
#         return output


def classify(model, x):
    """
    :param model:    network model object
    :param x:        (batch_size, 1, 28, 28) tensor - batch of images to classify

    :return labels:  (batch_size, ) torch tensor with class labels
    """

    output = model.forward(x)

    _, labels = torch.max(output,1)

    return labels


if __name__ == '__main__':
    pass
