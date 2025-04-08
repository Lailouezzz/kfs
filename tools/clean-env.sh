#!/usr/bin/sh

KFS_ROOT="$(dirname "$(dirname "$(realpath "$0")")")"
cd "$KFS_ROOT"

rm -rf env .vscode/{settings,tasks}.json
