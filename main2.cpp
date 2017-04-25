

#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <signal.h>
#include <fstream>
#include <wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <sstream>

using namespace std;

/**
 * Zerlegt den Befehl und Argumente
   * @param buf Zeiger auf den Befehl
 * @param args Zeiger auf Argumente
 */
void parse(char *buf, char **args) {
    while (*buf != NULL) {

        while ((*buf == ' ') || (*buf == '\t'))
              *buf++ = NULL;

        //Argumente speichern.
        *args++ = buf;

        //Springe über die Argumente
        while ((*buf != NULL) && (*buf != ' ') && (*buf != '\t'))
              buf++;
    }

    *args = NULL;
}

/**
 * Führt den Befehl aus und startet wenn nötig einen weiteren Prozess
 * @param args Argumente
 */
void execute(char **args) {
    int pid = 0;
    int stopp = 1;

    //letztes Zeichen suchen
    char **temp = args;
    while (*temp != NULL) {
        *temp++;
    }
    *temp--;

    //wenn letztes Zeichen ein "&" ist
      //--> löschen und nicht auf Kind-Prozess warten (stopp=0)
    if (**temp == '&') {
        *temp = NULL;
        stopp = 0;
    }

    //Kind-Prozess starten und Prozess-ID speichern
      if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    }

    //nur Kind-Prozess führt Befehl aus
    if (pid == 0) {
        execvp(*args, args);
        perror(*args);
          exit(1);
    }

    if (stopp == 1) {
        //Vater-Prozess wartet auf Kind-Prozess
        waitpid(pid, NULL, 0);
    } else {
        //Ausgabe der Prozess-ID
        //Kind-Prozess läuft im Hintergrund
          cout << "[" << pid << "]" << endl;
    }
}

void beenden(int param) {
    printf("nbye!n");
    exit(0);
}

main() {
    fstream log;

    signal(SIGCHLD, SIG_IGN);
    signal(SIGINT, beenden);

    printf("beenden mit CTR_Cn");

    char buf[1024];
    char *args[64];

    while (true) {
        printf("$ ");

        while (gets(buf) == NULL) {
            printf("n");
        }

        //Befehl in Log-Datei protokollieren
        log.open("logfile", fstream::out | fstream::app);
        log << buf << endl;
          log.close();

        //Befehl zerlegen
        parse(buf, args);

        //Befehl ausführen
        execute(args);
    }
}