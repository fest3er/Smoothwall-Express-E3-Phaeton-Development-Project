//Please refer to http://dansguardian.org/?page=copyright2
//for the license for this code.
//Written by Daniel Barron (daniel@ /jadeb.com).
//For support go to http://groups.yahoo.com/group/dansguardian

//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#ifndef __HPP_UDSOCKET
#define __HPP_UDSOCKET
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <exception>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string> // NEEDED!!!

using namespace std;

class UDSocket {

    public:
        UDSocket();
        ~UDSocket();
        UDSocket(int newfd, struct sockaddr_un myadr);

        void reset();
        int connect(const char* path);
        int bind(const char* path);
        int listen(int queue);
        UDSocket accept();

        int getFD();
        void setFD(int newfd);
        void close();
        void setTimeout(int t);
        int getTimeout();

        bool checkForInput();
        void checkForInput(int timeout) throw(exception);
        bool readyForOutput();
        void readyForOutput(int timeout) throw(exception);
        int getline(char* buff, int size, int timeout) throw(exception);
        void writeString(const char* line) throw(exception);
        bool writeToSocket(char* buff, int len, unsigned int flags, int timeout);
        int readFromSocketn(char* buff, int len, unsigned int flags, int timeout);
        int readFromSocket(char* buff, int len, unsigned int flags, int timeout);
        void writeToSockete(char* buff, int len, unsigned int flags, int timeout) throw(exception);


    private:
        int timeout;
        bool isclosed;
        bool isused;
        socklen_t my_adr_un_length;
        int sck_un;
        struct sockaddr_un my_adr_un;
        struct sockaddr_un peer_adr_un;
        int selectEINTR(int numfds, fd_set * readfds, fd_set * writefds, fd_set * exceptfds, struct timeval * timeout);
};

#endif
