#!/bin/bash

/opt/ssh-arcadium/bin/game/server &
/usr/sbin/sshd -D &

ssh -t -p 443 \
  -o StrictHostKeyChecking=no \
  -o ServerAliveInterval=30 \
  -R0:localhost:2222 \
  -L0.0.0.0:4300:localhost:4300 \
  tcp@a.pinggy.io &

sleep 5

TUNNEL_URL=$(curl -s http://localhost:4300/urls | grep -o 'tcp://[^"]*' | head -n 1)

if [ -n "$TUNNEL_URL" ]; then
  HOST=$(echo $TUNNEL_URL | sed 's|tcp://||' | cut -d: -f1)
  PORT=$(echo $TUNNEL_URL | sed 's|tcp://||' | cut -d: -f2)
  echo ""
  echo "SSH Arcadium is ready!"
  echo ""
  echo "Connect with:"
  echo "ssh -p $PORT player@$HOST"
else
  echo "Failed to get pinggy URL."
fi

tail -f /dev/null
