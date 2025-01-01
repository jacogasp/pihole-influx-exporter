#!/bin/bash

USERNAME=${1}
USER_UID=${2}
USER_GID=${3}

set -e

if [ "$(id -u)" -ne 0 ]; then
    echo -e 'Script must be run as root. Use sudo, su, or add "USER root" to your Dockerfile before running this script.'
    exit 1
fi

groupadd --gid $USER_GID $USERNAME
useradd --uid $USER_UID --gid $USER_GID -m $USERNAME

mkdir -p /etc/sudoers.d/

echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME
chmod 0440 /etc/sudoers.d/$USERNAME

USER_RC_PATH="/home/${USERNAME}"
chown ${USERNAME}:${USER_GID} "${USER_RC_PATH}/.bashrc"

echo "Done!"
