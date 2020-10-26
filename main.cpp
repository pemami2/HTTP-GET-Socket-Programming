#include <iostream>
#include <ctype.h>
#include <cstring>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <string>
#include <chrono>
#include <limits>
using namespace std;
using namespace std::chrono;

int socketRequest(char hostname[], char path[], double& size){
    int sock;
    struct sockaddr_in client;
    int PORT = 80;
    // converting DNS
    struct hostent * host = gethostbyname(hostname);
    // check if valid
    if ( (host == NULL) || (host->h_addr == NULL) ) {
        cout << "Error retrieving DNS information." << endl;
        exit(1);
    }

    bzero(&client, sizeof(client));
    client.sin_family = AF_INET;
    client.sin_port = htons( PORT );
    memcpy(&client.sin_addr, host->h_addr, host->h_length);
    
    // creating socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0) {
        cout << "Error creating socket." << endl;
        exit(1);
    }
    
    //connecting to server
    if ( connect(sock, (struct sockaddr *)&client, sizeof(client)) < 0 ) {
        close(sock);
        cout << "Could not connect" << endl;
        exit(1);
    }
    // preparing GET request
    stringstream ss;
    ss << "GET " << path << " HTTP/1.1\r\nHost: "<< hostname << "\r\nConnection: close\r\n\r\n";

    string request = ss.str();
    // sending request
    if (send(sock, request.c_str(), request.length(), 0) != (int)request.length()) {
        cout << "Error sending request." << endl;
        exit(1);
    }
    
    // reading response
    int count = 0;
    char cur;
    string status;
    while ( read(sock, &cur, 1) > 0 ) {
        if (count < 12 && count > 8 ){
            status+= cur;
        }
        cout << cur;
        size+=sizeof(cur);
        count++;
    }
    
    return stoi(status);

};

int main(int argc, char *argv[])
{
    //initializing variables
    string help = "\nUsage: [URL] [Number of Requests]\n\nURL is a mandatory option\nIf Number of Arugements is left blank the default number will be 1\nUsage: [--help] Prints this prompt \n\nWelcome to Parsa's HTTP GET request tool! This program will make a GET request to my Cloudflare Workers site (my-app.parsae.workers.dev) by default. In order to send a custom request, just pass in the URL corresponding to the site/endpoint you would like to hit followed by the number (default is 1) of requests you would like to make. Happy testing! \n";

    int status = 0;
    char hostname[80];
    char  path[80] ="/";
    int position = -1;
    double size = 0;
    double min_size = std::numeric_limits<double>::max();
    double max_size = 0;
    int n = 1;
    double median = 0;
    int ok = 0;
    int fail = 0;
    double times[n];
    double time_sum = 0;
    
    // taking in appropriate arguements
    if (argc > 1){
        if (string(argv[1]) == "--help"){
            cout << help << endl;
            exit(0);
            
        } else {
            // updating hostname from passed in URL
            string url = argv[1];
            position = url.find("/");
            
            // updating path from passed in URL
            if ( position >= 0){
                string host = url.substr(0,position);
                strcpy(hostname, host.c_str());
                string endpoint = url.substr(position);
                strcpy(path, endpoint.c_str());
            } else {
                strcpy(hostname, url.c_str());
            }
        }
    } else {
        cout << "\nError: No URL detected in first arguement\n\nUsage: [URL] [Number of Requests]\n" << endl;
        exit(1);
    }
    
    // determining number of responses from passed in arguement
    if (argc > 2){
        n = atoi(argv[2]);
    }
    
    // finding midpoint in array of request times
    int mid = n/2;
    
    // making n requests
    for ( int i = 0; i<n; i++){
        size = 0;
        auto start = high_resolution_clock::now();
        status = socketRequest(hostname, path, size);
        auto end = high_resolution_clock::now();
        auto time = duration_cast<microseconds>(end-start);
        double my_time= time.count();
        cout<<"\n\n";
        times[i] = my_time;
        time_sum+= my_time;
        
        // determining response succes from status code
        if (status >= 200  && status < 300){
            ok++;
        } else {
            fail++;
        }
        
        // finding minimum and maximum response times
        if (size > max_size){
            max_size = size;
        }
        
        if ( size < min_size){
            min_size = size;
        }

    }
    
    // calculating and outputting request data
    sort(times, times+n);
    
    // determine median time
    if (n%2 == 0){
        median = (times[mid] + times[mid-1])/2;
    } else {
        median = times[mid];
    }
    
    // displaying HTTP request metrics
    cout << n << " request(s)" << endl;
    
    for ( int i = 0; i<n; i++){
        cout << "time "<< i << " : "<< times[i] << endl;
    }
    
    cout << "mean time: " << time_sum/n << endl;
    cout << "median time: " << median << endl;
    cout << "minimum time: " << times[0] << endl;
    cout << "maximum time: " << times[n-1] << endl;
    cout << "successful responses: " << (ok/n)*100 << "%" << endl;
    cout << "unsuccessful responses: " << (fail/n)*100 << "%" << endl;
    cout << "largest response: " << max_size << " bytes" << endl;
    cout << "smallest response: " << min_size << " bytes" << endl;
}
