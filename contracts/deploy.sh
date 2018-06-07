scp contracts/ipfs.cpp  eos1:/root/projects/ipfs

ssh -T eos1 <<EOF
  cd projects/ipfs
  deploy.sh ipfs.app ipfs
EOF