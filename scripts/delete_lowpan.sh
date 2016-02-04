#!/bin/bash
if [[ $EUID -ne 0 ]]; then
  echo "You must be a root user" 2>&1
  exit 1
fi

ip link set wpan0 down
if (ip link show lowpan0 ) ; then
  ip link set lowpan0 down
  ip link del lowpan0 type lowpan
fi
ip link set wpan0 up
