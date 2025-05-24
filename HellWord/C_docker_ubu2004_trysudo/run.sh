#!/bin/bash

# Set the container name (adjust as needed)
CONTAINER_NAME="my_container_sudo_try0c"

# Check for arguments
if [ $# -lt 1 ]; then
    echo "Usage: $0 {up|down|exec <command>}"
    exit 1
fi

COMMAND=$1
shift # Move to next argument

case "$COMMAND" in
    up)
        echo "Starting containers..."
        docker-compose up -d
        ;;
    down)
        echo "Stopping containers..."
        docker-compose down
        ;;
    exec)
        if [ $# -eq 0 ]; then
            echo "Usage: $0 exec <command>"
            exit 1
        fi
        echo "Executing command inside container: $CONTAINER_NAME"
        docker exec -it "$CONTAINER_NAME" "$@"
        ;;
    *)
        echo "Invalid command! Use: up, down, or exec."
        exit 1
        ;;
esac

