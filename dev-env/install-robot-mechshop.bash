#!/bin/bash

set -e

# 1. Update and Upgrade
echo "Updating system..."
sudo apt update && sudo apt upgrade -y

# 2. Set Locale
echo "Setting locale..."
sudo locale-gen en_US en_US.UTF-8
sudo update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8
export LANG=en_US.UTF-8

# 3. Add ROS 2 GPG Key and Repo
echo "Adding ROS 2 repo..."
sudo apt install -y software-properties-common curl gnupg lsb-release
sudo add-apt-repository universe
curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key | \
  sudo gpg --dearmor -o /usr/share/keyrings/ros-archive-keyring.gpg

echo "deb [signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] http://packages.ros.org/ros2/ubuntu $(lsb_release -cs) main" | \
  sudo tee /etc/apt/sources.list.d/ros2.list

sudo apt update

# 4. Install ROS 2 Humble
echo "Installing ROS 2 Humble desktop..."
sudo apt install -y ros-humble-desktop

# 5. Source ROS in bashrc
echo "Setting up ROS environment..."
echo "source /opt/ros/humble/setup.bash" >> ~/.bashrc
source ~/.bashrc

# 6. Install Dev Tools and Init rosdep
echo "Installing ROS tools..."
sudo apt install -y python3-colcon-common-extensions python3-rosdep python3-vcstool git

echo "Initializing rosdep..."
sudo rosdep init || true
rosdep update

# 7. Install Gazebo Fortress
echo "Installing Gazebo Fortress..."
sudo apt install -y gazebo11 libgazebo11-dev

# 8. Install ROS-Gazebo integration
echo "Installing ROS-Gazebo bridge..."
sudo apt install -y ros-humble-gazebo-ros-pkgs ros-humble-gazebo-ros2-control

# 9. GUI support (optional for WSL 2 Windows 10 users)
if grep -q "Microsoft" /proc/version && ! grep -q "WSLg" /proc/version; then
  echo "Adding DISPLAY environment variable for GUI..."
  echo "export DISPLAY=:0" >> ~/.bashrc
  export DISPLAY=:0
fi

echo "✅ ROS 2 and Gazebo installation complete!"
echo "You can now test with: ros2 run demo_nodes_cpp talker"
