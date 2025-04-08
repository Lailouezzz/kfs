#!/usr/bin/sh

TEMPLATE_DIR="$(pwd)/tools/templates"

function construct_file()
{
  sed                                           \
    -e "s#__TEMPLATE_SHELL_PATH__#$SHELL#g"     \
    -e "s#__TEMPLATE_KFS_ROOT__#$KFS_ROOT#g"    \
    -e "s#__TEMPLATE_OLD_ZDOTDIR__#$ZDOTDIR#g"  \
    "$TEMPLATE_DIR"/"$1".template               \
    > "$2"
}

function configure_with_zsh()
{
  construct_file zsh/settings.json .vscode/settings.json
  construct_file zsh/tasks.json .vscode/tasks.json
  construct_file zsh/.zshrc env/.zshrc
  construct_file zsh/.zshenv env/.zshenv
  chmod +x env/.zshrc
  chmod +x env/.zshenv
}

function configure_with_bash()
{
  construct_file bash/settings.json .vscode/settings.json
  construct_file bash/tasks.json .vscode/tasks.json
  construct_file bash/.bashrc env/.bashrc
  construct_file bash/.bashenv env/.bashenv
  chmod +x env/.bashrc
  chmod +x env/.bashenv
}

function configure_vscode()
{
  case $(basename "$SHELL") in
  zsh) configure_with_zsh ;;
  bash) configure_with_bash ;;
  *)
    echo "No configuration will be done:" >&2
    echo -e "\tUnknown shell ($SHELL)" >&2
    exit 1
    ;;
  esac

  echo "HINT: If you are within VSCode, please restart VSCode's shell."
}

KFS_ROOT="$(dirname "$(dirname "$(realpath "$0")")")"
cd "$KFS_ROOT"
mkdir -p env

construct_file env env/env

for arg in "$@"; do
  case $arg in
    --no-vscode) NO_VSCODE= ;;
  esac
done

if [ ! -v NO_VSCODE ]; then
  configure_vscode
fi
