package main

import (
    "log"
    "net/http"
    "net/http/httputil"
    "net/url"
)

var serverCount = 0

// These constants are used to define all backend servers
const (
    SERVER1 = "https://www.google.com"
    SERVER2 = "https://www.facebook.com"
    SERVER3 = "https://www.yahoo.com"
    PORT = "1338"
)

// Requests are redirected to the appropriate URLs
func loadBalancer(w http.ResponseWriter, r *http.Request) {

    /* Get the address of one of the servers from this load
     * balancer to redirect/forward a request
     */
    url := getProxyURL()

    // Log the request
    logRequestPayload(url)

    /* Serve client with the response obtained from the fleet
     * of server
     */
    serveReverseProxy(url, w, r)
}

/* Load Balancer is a type of reverse proxy with more
 * functionalities like traffic management, etc.
 */
func main() {

    /* Start load balancer or
     * reverse proxy or
     * TCP server
     * ...whatever you say!
     */
    http.HandleFunc("/", loadBalancer)
    println("Visit http://localhost:"+PORT)
    log.Fatal(http.ListenAndServe(":" + PORT, nil))
}


/* Returns a URL of one of the servers from the fleet
 * using Round-Robin Algo.
 */
func getProxyURL() string {

     var servers = []string {SERVER1, SERVER2, SERVER3}
     server := servers[serverCount]
     serverCount++

    /* Reset the variable `serverCount` if it exceeds the
     * length of the number of servers in our fleet
     */
     if serverCount >= len(servers) {
         serverCount = 0
     }

     return server
}

// Logs the request obtain at loadbalancer
func logRequestPayload(proxyURL string) {

    log.Printf("Proxy_URL: %s\n", proxyURL)
}

/* Serve as reverse proxy for the server
 * with the URL obtained from `getProxyURL` func
 */
func serveReverseProxy(target string, w http.ResponseWriter, r *http.Request) {

    // Parse the URL
    url, _ := url.Parse(target)

    // Create the reverse proxy
    proxy := httputil.NewSingleHostReverseProxy(url)

    /* `ServeHTTP` is a method to the type of the var proxy
     *  which is non-blocking, since it uses a go routine
     */
    proxy.ServeHTTP(w, r)
}
