#!/bin/bash
echo "🔧 Syncing with GitHub..."
git pull origin main --rebase

echo "📦 Staging your new problems and emails..."
git add .

MESSAGE=${1:-"Daily Math Update"}
git commit -m "$MESSAGE"

echo "🚀 Deploying to daily-math.org..."
git push origin main

echo "✅ Success! Site updated."
