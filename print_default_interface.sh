ip -4 route ls | grep default | grep -Po '(?<=dev )(\S+)'
