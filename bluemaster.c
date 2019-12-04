#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <glib.h>
#include "bluemaster.h"

#define MAC_SIZE 20
#define INTERFACE_NAME_SIZE 10

static GMainLoop *g_loop;
static pthread_t g_thread;
static char g_target_addr[MAC_SIZE];
static char g_trusted_addr[MAC_SIZE];
static char g_interface[INTERFACE_NAME_SIZE];

static char g_change_mac[PATH_MAX];
static char g_change_cod[PATH_MAX];
static char g_connectable[PATH_MAX];
static char g_sdp[PATH_MAX];

char *g_trusted_type;
char *g_trusted_cod;

static GHashTable *g_cod_table;

void* _show(void *user_data);

int kbhit()
{
    struct termios oldt, newt;
    int ch;
    int oldf = 0;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_iflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}


char *fgetstr(char *string, int n, FILE *stream)
{
    char *result;

    result = fgets(string, n, stream);

    if (!result)
        return result;

    if (string[strlen(string) - 1] == '\n')
        string[strlen(string) - 1] = 0;

    return string;
}


void remove_thread()
{
    if (g_thread) {
        pthread_cancel(g_thread);
        pthread_join(g_thread, NULL);
        g_thread = 0;
    }
}


void show_menu(void *user_data)
{
    remove_thread();

    pthread_create(&g_thread, NULL, _show, user_data);
}


void get_information()
{
    PRINT_BROWN("Get the interface list of the bluetooth\n");
    system("hciconfig -a");

    PRINT_BROWN("Select the interface you want to use(e.g., hci0, hci1, ..)");
    fgetstr(g_interface, INTERFACE_NAME_SIZE, stdin);
    PRINT_BROWN("%s is selected.\n", g_interface);
}


void set_information()
{
    int cod;

    PRINT_BROWN("\nEnter the target device to identify the vulnerability");
    PRINT_BROWN("and the trusted device associated with that device.\n");
    PRINT(" - MAC Address of Target Device (e.g., AA:AA:AA:AA:AA:AA) :");
    fgetstr(g_target_addr, MAC_SIZE, stdin);
    PRINT(" - MAC Address of Trusted Device (e.g., BB:BB:BB:BB:BB:BB) :");
    fgetstr(g_trusted_addr, MAC_SIZE, stdin);

    PRINT_BROWN("Target Device: %s", g_target_addr);
    PRINT_BROWN("Trusted Device: %s", g_trusted_addr);

    PRINT(" - Type of Trusted Device");
    PRINT("   1. Phone");
    PRINT("   2. Smart Watch");
    PRINT("   3. Laptop");

    scanf("%d", &cod);

    switch (cod) {
        case 1:
            g_trusted_type = "PHONE";
            break;
        case 2:
            g_trusted_type = "WATCH";
            break;
        case 3:
            g_trusted_type = "LAPTOP";
            break;
        default:
            PRINT_RED("Not Supported Type");
            return;
    }

    g_trusted_cod = g_hash_table_lookup(g_cod_table, g_trusted_type);
}


void set_mac_address()
{
    PRINT_BROWN("Change MAC Address of this device to that of the trusted device.");
    PRINT(" This uses bdaddr from the bluez, and the setting method may differ");
    PRINT(" depending on the manufacturer.");
    PRINT("  e.g., CSR: After changing the MAC address,");
    PRINT("             remove the controller from the device and reconnect it.");
    PRINT("        Broadcom: If you remove the controller from the device");
    PRINT("             after changing the MAC address, the MAC address is also reset.");
    PRINT(" == Press Enter\n");

    while(!kbhit());

    strcpy(g_change_mac, "sudo ");
    strcat(g_change_mac, BLUEZ_DIR);
    strcat(g_change_mac, "/bdaddr -i ");
    strcat(g_change_mac, g_interface);
    strcat(g_change_mac, " ");
    strcat(g_change_mac, g_target_addr);
    system(g_change_mac);
}


void set_class_of_device()
{
    PRINT_BROWN("Change Class of Device");
    PRINT(" Change the type of your device to %s(%s)", g_trusted_type, g_trusted_cod);

    strcpy(g_change_cod, "sudo ");
    strcat(g_change_cod, "hciconfig ");
    strcat(g_change_cod, g_interface);
    strcat(g_change_cod, " class ");
    strcat(g_change_cod, g_trusted_cod);
    system(g_change_cod);

    system("hciconfig -a");
}


void set_connectable()
{
    PRINT_BROWN("Change the device to connectable");
    PRINT(" Set the state of the device to connectable");
    PRINT(" to ensure it is secure from passive attacks.");

    strcpy(g_connectable, "sudo ");
    strcat(g_connectable, "hciconfig ");
    strcat(g_connectable, g_interface);
    strcat(g_connectable, " pscan");
    system(g_connectable);
}


void sdp()
{
    PRINT("SDP");

    strcpy(g_sdp, "sdptool -i ");
    strcat(g_sdp, g_trusted_addr);
    strcat(g_sdp, " browse ");
    strcat(g_sdp, g_target_addr);

    while (!kbhit()) {

        g_usleep(100000);

        system(g_sdp);
    }


}


void* _show(void *user_data)
{
    int input = -1;

    do {
        PRINT("====================================================\n");
        PRINT("-  1. Get Information of Bluetooth Interface\n");
        PRINT("-  2. Set Device Information\n");
        PRINT("-  3. Change MAC Address (g_change_mac)\n");
        PRINT("-  4. Change Device of Class\n");
        PRINT("-  5. Change Connectable\n");
        PRINT("-  6. Service Discovery Protocol\n");
        PRINT("-  7. EXIT\n");
        PRINT("====================================================\n");

        PRINT("input: ");
        if (0 == scanf("%d", &input)) {
            PRINT_RED("scanf() Failed");
            break;
        }
        getchar();

        switch (input) {
            case 1:
                get_information();
                break;
            case 2:
                set_information();
                break;
            case 3:
                set_mac_address();
                break;
            case 4:
                set_class_of_device();
                break;
            case 5:
                set_connectable();
                break;
            case 6:
                sdp();
                break;
            default:
                PRINT_RED("Invalid Input (%d)", input);
                break;
        }



    } while (7 != input);

    g_main_loop_quit(g_loop);

    pthread_exit(NULL);

}

void set_cod_table()
{
    g_cod_table = g_hash_table_new_full(g_str_hash, g_str_equal, free, free);

    g_hash_table_insert(g_cod_table, g_strdup("PHONE"), g_strdup("0x000204"));
    g_hash_table_insert(g_cod_table, g_strdup("WATCH"), g_strdup("0x240704"));
    g_hash_table_insert(g_cod_table, g_strdup("LAPTOP"), g_strdup("0x02010c"));
}

int main()
{
    PRINT("START");

    g_loop = g_main_loop_new(NULL, FALSE);

    set_cod_table();

    pthread_create(&g_thread, NULL, _show, NULL);

    g_main_loop_run(g_loop);
    g_main_loop_unref(g_loop);

    g_hash_table_unref(g_cod_table);

    return 0;
}
