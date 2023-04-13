#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <libssh/libssh.h>

int main() {
    ssh_session session;
    int rc;

    // Initialize SSH library
    ssh_init();

    // Connect to remote host
    session = ssh_new();
    ssh_options_set(session, SSH_OPTIONS_HOST, "remote-host");
    ssh_options_set(session, SSH_OPTIONS_USER, "remote-user");
    rc = ssh_connect(session);

    if (rc != SSH_OK) {
        fprintf(stderr, "Error connecting to remote host: %s\n", ssh_get_error(session));
        ssh_free(session);
        return 1;
    }

    // Authenticate with remote host
    rc = ssh_userauth_password(session, NULL, "password");

    if (rc != SSH_AUTH_SUCCESS) {
        fprintf(stderr, "Error authenticating with remote host: %s\n", ssh_get_error(session));
        ssh_disconnect(session);
        ssh_free(session);
        return 1;
    }

    // Execute remote command
    ssh_channel channel = ssh_channel_new(session);
    rc = ssh_channel_open_session(channel);

    if (rc != SSH_OK) {
        fprintf(stderr, "Error opening channel: %s\n", ssh_get_error(session));
        ssh_channel_free(channel);
        ssh_disconnect(session);
        ssh_free(session);
        return 1;
    }

    rc = ssh_channel_request_exec(channel, "ls -la");

    if (rc != SSH_OK) {
        fprintf(stderr, "Error executing command: %s\n", ssh_get_error(session));
        ssh_channel_free(channel);
        ssh_disconnect(session);
        ssh_free(session);
        return 1;
    }

    char buffer[256];
    int nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);

    while (nbytes > 0) {
        if (fwrite(buffer, 1, nbytes, stdout) != nbytes) {
            fprintf(stderr, "Error writing to stdout.\n");
            ssh_channel_free(channel);
            ssh_disconnect(session);
            ssh_free(session);
            return 1;
        }

        nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
    }

    if (nbytes < 0) {
        fprintf(stderr, "Error reading from channel: %s\n", ssh_get_error(session));
        ssh_channel_free(channel);
        ssh_disconnect(session);
        ssh_free(session);
        return 1;
    }

    // Close SSH connection
    ssh_channel_send_eof(channel);
    ssh_channel_close(channel);
    ssh_channel_free(channel);
    ssh_disconnect(session);
    ssh_free(session);

    return 0;
}
