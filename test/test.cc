#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
  int h = socket(PF_INET, SOCK_STREAM, 0);
  if (h < 0) {
    perror("creating socket");
  }
  const int PORT = 5678;
  struct sockaddr_in local;
  // inet_aton(
  local.sin_family = PF_INET;
  local.sin_port = htons(PORT);
  if (bind(h, (sockaddr *)&local, sizeof local) < 0) {
    perror("bind");
	close(h);
	return 1;
  }
  if (listen(h, 10) < 0) {
    perror("listen"); close(h); return 2;
  }
	char buf[512] = "";
	int connect_socket;
	struct sockaddr_in remote;
	unsigned remoteLen = sizeof remote;
	connect_socket = accept(h, (sockaddr *)&remote, &remoteLen);
//	printf("Accepted!\n");
//	char buf[512];
	int rd;
	while((rd = read(connect_socket, buf, sizeof(buf)-1)) > 0) {
        	buf[rd]='\0';    // explicit null termination: updated based on comments
        	printf("%s\n",buf); // print the current receive buffer with a newline
        	fflush(stdout);         // make sure everything makes it to the output
        	buf[0]='\0';        // clear the buffer : I am 99% sure this is not needed now
    	}
//	printf("read %d bytes '%s'\n", rd, buf);
	close(connect_socket);
  close(h);
}
  
