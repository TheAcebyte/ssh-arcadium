#!/bin/bash

if [ -n "$NGROK_AUTHTOKEN" ]; then
    ngrok config add-authtoken $NGROK_AUTHTOKEN
fi

/usr/sbin/sshd -D &

ngrok tcp 22 &

sleep 3

NGROK_URL=$(curl -s http://localhost:4040/api/tunnels | grep -o '"public_url":"[^"]*' | grep -o 'tcp://[^"]*' | head -n 1)

if [ -n "$NGROK_URL" ]; then
    HOST=$(echo $NGROK_URL | sed 's|tcp://||' | cut -d: -f1)
    PORT=$(echo $NGROK_URL | sed 's|tcp://||' | cut -d: -f2)
    echo "SSH Arcadium is ready!"
    echo ""
    echo "Connect with:"
    echo "ssh -p $PORT player@$HOST"
else
    echo "Failed to get ngrok URL."
fi

tail -f /dev/null
