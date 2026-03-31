#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

# Define colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${YELLOW}Starting Diary installation process...${NC}\n"

if ! command -v pio &> /dev/null; then
    echo -e "${RED}Error: PlatformIO (pio) could not be found.${NC}"
    echo "Please install it first: https://docs.platformio.org/en/latest/core/installation.html"
    exit 1
fi

if [ ! -f "platformio.ini" ]; then
    echo -e "${RED}Error: platformio.ini not found.${NC}"
    echo "Please run this script from the root of the project directory."
    exit 1
fi

echo -e "${GREEN}==> Compiling and uploading firmware...${NC}"
pio run -t upload

echo -e "\n${GREEN}==> Building and uploading filesystem image (LittleFS)...${NC}"
pio run -t uploadfs

echo -e "\n${GREEN}Success! The firmware and filesystem have been uploaded.${NC}"
