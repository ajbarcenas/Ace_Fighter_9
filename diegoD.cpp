// Name: Diego Diaz
// Date: 9/17/2019
// The four steps of debugging are to identify the bug, isolate the source of the bug, fix the bug, and review the bug.
#include <stdio.h>
#include <string.h>
#include <GL/glx.h>
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <iostream>
using namespace std;
#define PORT 443
#define USERAGENT "CMPS-3350"

const int  MAX_READ_ERRORS = 100;

struct Vec {
	float x,y,z;
};

struct Shape {
	float width, height;
	float radius;
	Vec center;
	Vec velocity;
	bool playerExists = false;
};

struct Enemy1 {
	int health;
	int damage;
	Shape s;
	bool removeEnemy = false;
};

struct Node
{
	Enemy1 data;
	struct Node *next;
};


void spawnPlayer(Shape *p)
{
	p->width = 25;
	p->height = 25;
	p->center.x = 200;
	p->center.y = 570;
}

void drawPlayer()
{

}

void checkPlayerLocation(Shape *p)
{
	if(p->center.x - p->width < -25 || p->center.x + p->width > 1945 ||
			p->center.y - p->height < -25 || p->center.y + p->height > 1105) {
		spawnPlayer(p);
	}
}

// =========================Enemy Functions =================================
/*
   void removeEnemy(Shape *e, int *i)
   {
   for(int j = 0; j < *i; j++){
   cout << *i << endl;
   e[j] = e[*i-1];
   (*i)--;
   }
   }

   void checkEnemyLocation(Shape *e, int *i)
   {	
   for(int j = 0; j < *i; j++){
//cout << e[j].center.x << endl;
if(e[j].center.x < 0.0){
//cout << "true" << endl;
removeEnemy(e, i);
}
}
}

void subtractEnemyHealth(struct Node* enemy, int damage)
{
enemy->data.health -= damage;
if(enemy.health <= 0){
removeEnemy(head_ref);
}
}

void subtractPlayerHealth(Shape player, int damage)
{
player.health -= damage;
if(player.health <= 0) {
gameOver();
}
}

*/

void spawnEnemy(struct Node** head_ref, Enemy1 enemy) 
{
	struct Node* new_node = (struct Node*) malloc(sizeof(struct Node));
	new_node->data = enemy;
	new_node->next = (*head_ref);
	(*head_ref) = new_node;
}

void setEnemySize(struct Node* head_ref, int i)
{
	head_ref->data.s.height = 18;
	head_ref->data.s.width = 18;
	head_ref->data.s.center.x = ((i+1) * 50) + 1500;
	head_ref->data.s.center.y = 900 - ((i+1) * 90);
}

void printEnemy(struct Node* temp)
{
	float we[5];
	float he[5];

	for (int i = 0; i < 5; i++) {
		glPushMatrix();
		glColor3ub(190,150,10);
		we[i] = temp->data.s.width;
		he[i] = temp->data.s.height;
		glTranslatef(temp->data.s.center.x,
				temp->data.s.center.y, temp->data.s.center.z);
		glBegin(GL_QUADS);
		glVertex2i(-we[i],-he[i]);
		glVertex2i(-we[i], he[i]);
		glVertex2i( we[i], he[i]);
		glVertex2i( we[i],-he[i]);
		glEnd();  
		glPopMatrix();
		temp = temp->next;
	}
}

void moveEnemy(struct Node* enemy) 
{
	enemy->data.s.velocity.x = -3.0;
	enemy->data.s.center.x += enemy->data.s.velocity.x;
}

void removeEnemy(struct Node** head, struct Node* enemy, int &n)
{
	/*struct Node* temp = head;
	  for(int i = 0; i < 5;i++){
	  if(head == enemy){
	  head = head->next;
	  return;	
	  }
	  else if(temp->next == enemy) {
	  if(temp->next->next != NULL) {
	  temp->next = temp->next->next;
	  }
	  else{
	  enemy = temp;
	  enemy->next = NULL;
	  }
	  }
	  }*/

	struct Node* temp = *head;
	struct Node* prev = *head;
	// If head node itself holds the key to be deleted 
	if (temp != NULL && temp == enemy) 
	{ 
		*head = temp->next;   // Changed head 
		free(temp);		// free old head 
		cout << "working" << endl;
		n--;
		return; 
	} 

	// Search for the key to be deleted, keep track of the 
	// previous node as we need to change 'prev->next' 
	while (temp != NULL && temp != enemy) 
	{ 
		prev = temp; 
		temp = temp->next; 
		cout << "This one is working" << endl;
	} 

	// If key was not present in linked list 
	if (temp == NULL) return; 

	// Unlink the node from linked list 
	prev->next = temp->next; 
	free(temp);	
	n--;
}

void checkEnemyLocation(struct Node* enemy)
{
	if(enemy->data.s.center.x < 100) {
		enemy->data.removeEnemy = true;
	}
}

void ShowDiegosPicture(int x, int y, GLuint textid)
{

}


//==============================****  High Score Functions ****==============================================//

BIO *ssl_setup_bio(void)
{
	//Setup the ssl BIO, basic I/O abstraction.
	//https://www.openssl.org/docs/man1.1.0/man3/bio.html
	BIO *bio = NULL;
	OpenSSL_add_all_algorithms();
	ERR_load_BIO_strings();
	ERR_load_crypto_strings();
	SSL_load_error_strings();
	bio = BIO_new(BIO_s_file());
	bio = BIO_new_fp(stdout, BIO_NOCLOSE);
	return bio;
}


int authScores()
{
	BIO *ssl_setup_bio(void);
	void show_cert_data(SSL *ssl, BIO *outbio, const char *hostname);
	void set_to_non_blocking(const int sock);
	int sd;
	struct hostent *host;
	struct sockaddr_in addr;
	BIO *outbio = NULL;
	// added const
	const SSL_METHOD *method;
	SSL_CTX *ctx;
	SSL *ssl;
	char req[1000];
	int req_len;
	char hostname[256] = "odin.cs.csub.edu";
	char pagename[256] = "/~abarcenas/3350/Game/HighScores.txt";
	int port = PORT;
	int bytes, nreads, nerrs;
	char buf[256];
	int ret;

	//
	//Setup the SSL BIO
	outbio = ssl_setup_bio();
	//Initialize the SSL library
	if(SSL_library_init() < 0)
		BIO_printf(outbio, "Could not initialize the OpenSSL library !\n");
	method = SSLv23_client_method();
	ctx = SSL_CTX_new(method);
	SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2);
	//next 2 lines of code are not currently needed.
	//SSL_MODE_AUTO_RETRY flag of the SSL_CTX_set_mode call.
	//SSL_CTX_set_mode(ctx, SSL_MODE_AUTO_RETRY);
	//
	//Setup the socket used for connection.
	host = gethostbyname(hostname);
	sd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = *(long*)(host->h_addr);
	if (connect(sd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
		BIO_printf(outbio, "%s: Cannot connect to host %s [%s] on port %d.\n",
				"highscore.cpp", hostname, inet_ntoa(addr.sin_addr), port);
	}
	//Connect using the SSL certificate.
	ssl = SSL_new(ctx);
	SSL_set_fd(ssl, sd);
	SSL_connect(ssl);
	//
	//Show the certificate data
	show_cert_data(ssl, outbio, hostname);
	//
	//A non-blocking socket will make the ssl_read() not block.
	set_to_non_blocking(sd);
	//
	//Send the http request to the host server.
	sprintf(req, "GET /%s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\n\r\n",
			pagename, hostname, USERAGENT);
	req_len = strlen(req);
	ret = SSL_write(ssl, req, req_len);
	// Added braces in K&R style
	if (ret <= 0) {
		fprintf(stderr, "ERROR: SSL_write\n"); fflush(stderr);
	}
	//
	//Get data returned from the server.
	//First, do priming read.
	//We can take this approach because our socket is non-blocking.
	//Start with an error condition.
	bytes = -1;
	memset(buf, '\0', sizeof(buf));
	while(bytes <= 0) {
		bytes = SSL_read(ssl, buf, sizeof(buf));
		//A slight pause can cause fewer reads to be needed.
		usleep(10000);
	}
	//A successful priming read was accomplished.
	//Now read all the data.
	nreads = 1;
	//Allow for some read errors to happen, while getting the complete data.
	nerrs = 0;
	while(bytes >= 0 && nerrs < MAX_READ_ERRORS) {
		write(STDOUT_FILENO, buf, bytes);
		memset(buf, '\0', sizeof(buf));
		++nreads;
		bytes = SSL_read(ssl, buf, sizeof(buf));
		if (bytes == 0) ++nerrs; else nerrs = 0;
		//A slight pause can cause fewer reads to be needed.
		usleep(20000);
	}
	printf("\nn calls to ssl_read(): %i\n", nreads); fflush(stdout);
	//Cleanup.
	SSL_free(ssl);
	close(sd);
	SSL_CTX_free(ctx);
	return 0;
}

void show_cert_data(SSL *ssl, BIO *outbio, const char *hostname)
{
	//Display ssl certificate data here.
	//Get the remote certificate into the X509 structure
	printf("--------------------------------------------------------------\n");
	printf("Certificate data...\n");
	X509 *cert;
	X509_NAME *certname;
	printf("calling SSL_get_peer_certificate(ssl)\n");
	cert = SSL_get_peer_certificate(ssl);
	if (cert == NULL)
		printf("Error: Could not get a certificate from: %s.\n", hostname);
	else
		printf("Retrieved the server's certificate from: %s.\n", hostname);
	//extract various certificate information
	certname = X509_NAME_new();
	certname = X509_get_subject_name(cert);
	//display the cert subject here
	if (BIO_printf(outbio, "Displaying the certificate subject data:\n") < 0)
		fprintf(stderr, "ERROR: BIO_printf\n");
	X509_NAME_print_ex(outbio, certname, 0, 0);
	if (BIO_printf(outbio, "\n\n") < 0)
		fprintf(stderr, "ERROR: BIO_printf\n");
	printf("--------------------------------------------------------------\n");
}

void set_to_non_blocking(const int sock)
{
	//Set a socket to be non-blocking.
	int opts;
	opts = fcntl(sock, F_GETFL);
	if (opts < 0) {
		perror("ERROR: fcntl(F_GETFL)");
		exit(EXIT_FAILURE);
	}
	opts = (opts | O_NONBLOCK);
	if (fcntl(sock, F_SETFL, opts) < 0) {
		perror("ERROR: fcntl(O_NONBLOCK)");
		exit(EXIT_FAILURE);
	}
}
