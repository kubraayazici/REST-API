#include <microhttpd.h>
#include <stdio.h>
#include <string.h>

#define PORT 8000
/// @brief 
/// @param page 
void post_method(){
    // page = "<html><body>Hello from POST request!</body></html>";
    printf("%s\n", "POST request received!"); // Print message to command line
}
static int request_handler(void *cls, struct MHD_Connection *connection,
                           const char *url, const char *method,
                           const char *version, const char *upload_data,
                           size_t *upload_data_size, void **con_cls) {
    const char *page;
    const char *post_received_msg = "POST request received!";
    const char *get_received_msg = "GET request received!";

    if (strcmp(method, "POST") == 0) {
        // Handle POST request
        post_method();
        page = "<html><body><form method=\"get\" action=\"/get\">"
               "<button type=\"submit\">Send GET request</button>"
               "</form></body></html>";
        printf("%s\n", post_received_msg); // Print message to command line
    }
    else if (strcmp(method, "GET") == 0) {
        // Handle GET request
        page = "<html><body><form method=\"post\" action=\"/post\">"
               "<button type=\"submit\">Send POST request</button>"
               "</form></body></html>";
        printf("%s\n", get_received_msg); // Print message to command line
    } 
    else if (strcmp(method, "DELETE") == 0) {
        // Handle DELETE request
        page = "<html><body>Hello from DELETE request!</body></html>";
        printf("Received DELETE request!\n"); // Print message to command line
    } else {
        // Handle other request methods (e.g., GET)
        page = "<html><body>Hello!</body></html>";
    }

    struct MHD_Response *response;
    int ret;

    response = MHD_create_response_from_buffer(strlen(page),
                                                (void *)page,
                                                MHD_RESPMEM_PERSISTENT);
    ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);
    return ret;
}


int main() {
    struct MHD_Daemon *daemon;

    daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL,
                               &request_handler, NULL, MHD_OPTION_END);
    if (NULL == daemon) {
        return 1;
    }

    printf("Server started on port %d\n", PORT);
    getchar(); // Press any key to stop the server

    MHD_stop_daemon(daemon);
    return 0;
}
