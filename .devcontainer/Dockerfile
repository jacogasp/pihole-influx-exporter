FROM almalinux:9

ARG USERNAME=vscode
ARG USER_UID=1000
ARG USER_GID=${USER_UID}

ENV VCPKG_ROOT=/vcpkg
ENV VCPKG_DOWNLOADS=/vcpkg_downloads
# Required when building for arch != x86. In any case, since we provide system
# ninja and cmake binaries, this flag works also for x86. See
# https://learn.microsoft.com/en-us/vcpkg/users/config-environment#vcpkg_force_system_binaries
ENV VCPKG_FORCE_SYSTEM_BINARIES=1
ENV VCPKG_DISABLE_METRICS=1

COPY ./scripts/ /tmp/scripts

RUN chmod +x /tmp/scripts/*.sh && \
  /tmp/scripts/provide-deps.sh && \
  /tmp/scripts/provide-user.sh ${USERNAME} ${USER_UID} ${USER_GID} && \
  /tmp/scripts/install-vcpkg.sh ${USERNAME} && \
  rm -rf /tmp/scripts
