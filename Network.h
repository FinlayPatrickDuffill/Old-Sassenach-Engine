#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL_net.h>
#include <SDL_test.h>
#include "LTexture.h"
#include "Network_client_macros.h"

IPaddress ip;
bool done = false;
IPaddress serverIP;

/* This is really easy.  All we do is monitor connections */

/* Global variables */
static TCPsocket tcpsock = NULL;
static UDPsocket udpsock = NULL;
static SDLNet_SocketSet socketset = NULL;
static UDPpacket **packets = NULL;
static struct {
    int active;
    Uint8 name[256+1];
} people[CHAT_MAXCLIENTS];
char *server;

static char keybuf[80-sizeof(CHAT_PROMPT)+1];
static int  keypos = 0;

#define FONT_LINE_HEIGHT    (FONT_CHARACTER_SIZE + 2)

void SendHello(const char *name)
{
    IPaddress *myip;
    char hello[1+1+256];
    int i, n;

    /* No people are active at first */
    for ( i=0; i<CHAT_MAXCLIENTS; ++i ) {
        people[i].active = 0;
    }
    if ( tcpsock != NULL ) {
        /* Get our chat handle */
        if ( (name == NULL) &&
             ((name=getenv("CHAT_USER")) == NULL) &&
             ((name=getenv("USER")) == NULL ) ) {
            name="Unknown";
        }
        printf("Using name '%s'\n", name);

        /* Construct the packet */
        hello[0] = CHAT_HELLO;
        myip = SDLNet_UDP_GetPeerAddress(udpsock, -1);
        memcpy(&hello[CHAT_HELLO_PORT], &myip->port, 2);
        if ( strlen(name) > 255 ) {
            n = 255;
        } else {
            n = strlen(name);
        }
        hello[CHAT_HELLO_NLEN] = n;
        strncpy(&hello[CHAT_HELLO_NAME], name, n);
        hello[CHAT_HELLO_NAME+n++] = 0;

        /* Send it to the server */
        SDLNet_TCP_Send(tcpsock, hello, CHAT_HELLO_NAME+n);
    }
    else
    {
        printf("TCP is empty, can't send hello\n");
    }
}

void SendBuf(char *buf, int len)
{
    int i;

    /* Redraw the prompt and add a newline to the buffer */
    buf[len++] = '\n';

    /* Send the text to each of our active channels */
    for ( i=0; i < CHAT_MAXCLIENTS; ++i ) {
        if ( people[i].active ) {
            if ( len > packets[0]->maxlen ) {
                len = packets[0]->maxlen;
            }
            memcpy(packets[0]->data, buf, len);
            packets[0]->len = len;
            SDLNet_UDP_Send(udpsock, i, packets[0]);
        }
    }
}

int HandleServerData(Uint8 *data)
{
    int used = 0;

    switch (data[0]) {
        case CHAT_ADD: {
            Uint8 which;
            IPaddress newip;

            /* Figure out which channel we got */
            which = data[CHAT_ADD_SLOT];
            if ((which >= CHAT_MAXCLIENTS) || people[which].active) {
                /* Invalid channel?? */
                break;
            }
            /* Get the client IP address */
            newip.host=SDLNet_Read32(&data[CHAT_ADD_HOST]);
            newip.port=SDLNet_Read16(&data[CHAT_ADD_PORT]);

            /* Copy name into channel */
            memcpy(people[which].name, &data[CHAT_ADD_NAME], 256);
            people[which].name[256] = 0;
             people[which].active = 1;

            /* Let the user know what happened */
            printf(
    "* New client on %d from %d.%d.%d.%d:%d (%s)\n", which,
        (newip.host>>24)&0xFF, (newip.host>>16)&0xFF,
            (newip.host>>8)&0xFF, newip.host&0xFF,
                    newip.port, people[which].name);

            /* Put the address back in network form */
            newip.host = SDL_SwapBE32(newip.host);
            newip.port = SDL_SwapBE16(newip.port);

            /* Bind the address to the UDP socket */
            SDLNet_UDP_Bind(udpsock, which, &newip);
        }
        used = CHAT_ADD_NAME+data[CHAT_ADD_NLEN];
        break;
        case CHAT_DEL: {
            Uint8 which;

            /* Figure out which channel we lost */
            which = data[CHAT_DEL_SLOT];
            if ( (which >= CHAT_MAXCLIENTS) ||
                        ! people[which].active ) {
                /* Invalid channel?? */
                break;
            }
            people[which].active = 0;

            /* Let the user know what happened */
            printf(
    "* Lost client on %d (%s)\n", which, people[which].name);

            /* Unbind the address on the UDP socket */
            SDLNet_UDP_Unbind(udpsock, which);
        }
        used = CHAT_DEL_LEN;
        break;
        case CHAT_BYE: {
            printf("* Chat server full\n");
        }
        used = CHAT_BYE_LEN;
        break;
        default: {
            /* Unknown packet type?? */;
        }
        used = 0;
        break;
    }
    return(used);
}

void HandleServer(void)
{
    Uint8 data[512];
    int pos, len;
    int used;

    /* Has the connection been lost with the server? */
    len = SDLNet_TCP_Recv(tcpsock, (char *)data, 512);
    if ( len <= 0 ) {
        SDLNet_TCP_DelSocket(socketset, tcpsock);
        SDLNet_TCP_Close(tcpsock);
        tcpsock = NULL;
        printf("Connection with server lost!\n");
    } else {
        pos = 0;
        while ( len > 0 ) {
            used = HandleServerData(&data[pos]);
            pos += used;
            len -= used;
            if ( used == 0 ) {
                /* We might lose data here.. oh well,
                   we got a corrupt packet from server
                 */
                len = 0;
            }
        }
    }
}
void HandleClient(void)
{
    int n;

    n = SDLNet_UDP_RecvV(udpsock, packets);
    while ( n-- > 0 ) {
        if ( packets[n]->channel >= 0 ) {
        }
    }
}

void HandleNet(void)
{
    SDLNet_CheckSockets(socketset, 0);
    if ( SDLNet_SocketReady(tcpsock) ) {
        HandleServer();
    }
    if ( SDLNet_SocketReady(udpsock) ) {
        HandleClient();
    }
}

// hardcoded for the moment
char* determine_server(char* servername ){

    server = "51.7.52.125";
    return servername;

}

// SDL_net_init
bool find_game_server(char *servername, Uint16 port, char* name, bool connecting_success){

    connecting_success = false;

    printf("Starting client...\n");

    server = servername;
    printf("server determined as %s\n",server);

    /* Allocate a vector of packets for client messages */
    packets = SDLNet_AllocPacketV(4, CHAT_PACKETSIZE);
    if ( packets == NULL ) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Couldn't allocate packets: Out of memory\n");
    }

    /* Connect to remote host and create UDP endpoint */
    printf("Connecting to %s:%d ... \n", server,port);
    SDLNet_ResolveHost(&serverIP, server, port);
    if ( serverIP.host == INADDR_NONE ) {
        printf("Couldn't resolve hostname ... \n");
    } else {
        /* If we fail, it's okay, the GUI shows the problem */
        tcpsock = SDLNet_TCP_Open(&serverIP);
        if ( tcpsock == NULL ) {
            printf("Connect failed ... \n");
        } else {
            printf("Connected ... \n");
        }
    }
    printf("Try ports in the range {CHAT_PORT - CHAT_PORT+10}... \n");
    for (int i=0; (udpsock == NULL) && i<port; ++i ) {
        udpsock = SDLNet_UDP_Open(port+i);
    }
    if ( udpsock == NULL ) {
        SDLNet_TCP_Close(tcpsock);
        tcpsock = NULL;
        printf("Couldn't create UDP endpoint\n");
    }

    printf("Allocate the socket set for polling the network...\n");
    socketset = SDLNet_AllocSocketSet(2);
    if ( socketset == NULL ) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Couldn't create socket set: %s\n",
                     SDLNet_GetError());
    }
    if (!SDLNet_TCP_AddSocket(socketset, tcpsock))
    {
        printf("Couldn't add TCP socket\n");
    }
    SDLNet_UDP_AddSocket(socketset, udpsock);

    IPaddress *myip;
    char hello[1+1+256];
    int i, n;



    /* No people are active at first */
    for ( i=0; i<CHAT_MAXCLIENTS; ++i ) {
        people[i].active = 0;
    }
    if ( tcpsock != NULL ) {
        /* Get our chat handle */
        if ( (name == NULL) &&
             ((name=getenv("CHAT_USER")) == NULL) &&
             ((name=getenv("USER")) == NULL ) ) {
            name="Unknown";
        }
        printf("Using name '%s'\n", name);

        /* Construct the packet */
        hello[0] = CHAT_HELLO;
        myip = SDLNet_UDP_GetPeerAddress(udpsock, -1);
        memcpy(&hello[CHAT_HELLO_PORT], &myip->port, 2);
        if ( strlen(name) > 255 ) {
            n = 255;
        } else {
            n = strlen(name);
        }
        hello[CHAT_HELLO_NLEN] = n;
        strncpy(&hello[CHAT_HELLO_NAME], "", n);
        hello[CHAT_HELLO_NAME+n++] = 0;

        /* Send it to the server */
        SDLNet_TCP_Send(tcpsock, hello, CHAT_HELLO_NAME+n);
        connecting_success = true;
        return connecting_success;
    }
    return connecting_success;
}

void soon_obsolete_hardcoded_network_test_function(gameworld_constants gameworld_constants_logic, SDL_Window* gWindow){
    // Network testing section of the spaghetti
	printf( "Able to initialize graphics SDL_net...\n" );
    // find the IP to connect to
    gameworld_constants_logic.connection_status = find_game_server("51.7.52.125", 1338, "Name", gameworld_constants_logic.connection_status);
    // Connecting to a client I guess

    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT );
    if (gameworld_constants_logic.connection_status == true)
    {
        dynamic_screenprint_function(150.f, 160.f, gameworld_constants_logic, "connection to the specified address successful");
    }
    else
    {
        dynamic_screenprint_function(150.f, 160.f, gameworld_constants_logic, "connection to the specified address unsuccessful");
    }
    SDL_GL_SwapWindow( gWindow );
    SDL_Delay(500);

    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT );
    dynamic_screenprint_function(150.f, 120.f, gameworld_constants_logic, "welcome to sassenach");
    dynamic_screenprint_function(120.f, 140.f, gameworld_constants_logic, "you can currently load from a clientside map or the server");
    dynamic_screenprint_function(150.f, 160.f, gameworld_constants_logic, "choose an option");
    dynamic_screenprint_function(150.f, 180.f, gameworld_constants_logic, "load from a client side map j");
    dynamic_screenprint_function(150.f, 200.f, gameworld_constants_logic, "load from a server side map k");
    SDL_GL_SwapWindow( gWindow );
}

#endif // NETWORK_H_INCLUDED
