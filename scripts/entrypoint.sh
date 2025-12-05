#!/bin/bash

TUNNEL_TYPE=${TUNNEL_TYPE:-ngrok}

/opt/ssh-arcadium/bin/game/server &
/usr/sbin/sshd -D &

if [ "$TUNNEL_TYPE" = "cloudflare" ]; then
    cloudflared tunnel --url tcp://localhost:22 &> /tmp/cloudflared.log &
    sleep 5
    
    TUNNEL_URL=$(grep -oE 'https://[a-z0-9-]+\.trycloudflare\.com' /tmp/cloudflared.log | head -n 1)
    
    if [ -n "$TUNNEL_URL" ]; then
        HOST=$(echo $TUNNEL_URL | sed 's|https://||')
        echo "SSH Arcadium is ready!"
        echo ""
        echo "Connect with:"
        echo "ssh -o ProxyCommand=\"cloudflared access ssh --hostname $HOST\" player@$HOST"
    else
        echo "Failed to get Cloudflare tunnel URL."
    fi
else
    if [ -n "$NGROK_AUTHTOKEN" ]; then
        ngrok config add-authtoken $NGROK_AUTHTOKEN
    fi
    
    ngrok tcp 22 &
    sleep 3
    
    NGROK_URL=$(curl -s http://localhost:4040/api/tunnels | grep -o '"public_url":"[^"]*"' | grep -o 'tcp://[^"]*' | head -n 1)
    
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
fi

tail -f /dev/null
