alias c='gcc -Wall -g3 cliente.c -o cliente'
alias s='gcc -Wall -g3 servidor.c -o servidor'
alias rc='gcc -Wall -g3 cliente.c -o cliente && \
  clear && ./cliente 127.0.0.1 1400'
alias rcc='gcc -Wall -g3 cliente.c -o cliente && \
  clear && ./cliente 127.0.0.1 9999'
alias rs='gcc -Wall -g3 servidor.c -o servidor && \
  clear && ./servidor 1400'
alias rss='gcc -Wall -g3 servidor.c -o servidor && \
  clear && ./servidor 9999'
