#!/bin/bash
echo "🔧 Starting the Daily Math Update..."
git pull origin main --rebase
git add .
MESSAGE=${1:-"Daily Math Update"}
git commit -m "$MESSAGE"
git push origin main
echo "✅ Success! Your changes are being deployed."
