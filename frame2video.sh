## This script converts a sequence of image frames into a video file using ffmpeg.
#!/bin/bash
# Check if ffmpeg is installed
if ! command -v ffmpeg &> /dev/null
then
    echo "ffmpeg could not be found. Please install ffmpeg to use this script."
    exit 1
fi

# Run ffmpeg to convert frames to video 
ffmpeg -framerate 30 -i ./results/frames/frame_%04d.bmp -vf "scale=800:-1:flags=lanczos" -c:v libx264 -crf 28 -preset medium -pix_fmt yuv420p ./results/simulation.mp4


# Check if the video was created successfully
if [ $? -eq 0 ]; then
    echo "Video created successfully: ./results/simulation.mp4"
else
    echo "Failed to create video."
    exit 1
fi

# Clean up frames if needed
rm ./results/frames/frame_*.bmp

# Optionally: convert any screenshot bmp to png 
ffmpeg -i results/screenshot.bmp -vf scale=800:-1 results/screenshot.png
