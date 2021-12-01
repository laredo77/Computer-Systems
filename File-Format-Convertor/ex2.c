// Itamar Laredo
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const char* unx = "-unix";
const char* win = "-win";
const char* mac = "-mac";
const char* swap = "-swap";
const char* keep = "-keep";
const char bom_1 = 0xFF;
const char bom_2 = 0xFE;

/*
 * Option 1: In this option the function copy the src file content
 * to destination file. if destination file exist, its overwrite
 * otherwise its open new destination file.
 */
void option_1(char* src_file, char* dst_file) {

    char buffer[2]; // Buffer to store data
    FILE *fp;       // Src file pointer
    FILE *fp2;      // Dest file pointer
    // Trying to open source file, if not exist - exit.
    if ((fp = fopen(src_file, "rb")) != NULL) {
        fp2 = fopen(dst_file, "wb");
        // Copy the content and write to the destination file
        while (fread(&buffer, sizeof(char), 1, fp) != 0) {
            fwrite(buffer, sizeof(char), 1, fp2);
        }
        fclose(fp);
        fclose(fp2);
    } else {
        exit(0);
    }
}

/*
 * Option 2: Given source and destination file, in addition given src file
 * format (unix/windows/mac) and dest file format. this function convert
 * the src content to the format needed to be written in the dst file and
 * write it there.
 */
void option_2(char* src_file, char* dst_file, char* from, char* to) {
    // If src file and dest file should be the same format, case option 1.
    if (strcmp(from, to) == 0) {
        option_1(src_file, dst_file);
    } else {
        char buffer[2]; // Buffer to store data
        int i;
        FILE *fp;  // Src file pointer
        FILE *fp2; // Dest file pointer
        // Trying to open source file, if not exist - exit.
        if ((fp = fopen(src_file, "rb")) != NULL) {
            fp2 = fopen(dst_file, "wb");
            // Reading the first 2 bytes to decide whether is little or big endian
            fread(&buffer, sizeof(char) * 2, 1, fp);
            if (buffer[0] == bom_1 && buffer[1] == bom_2) {
                i = 0;
            } else {
                i = 1;
            }
            // Write the bom to the destination file
            fwrite(buffer, sizeof(char) * 2, 1, fp2);
            /*
             * read the src file content and write it in the dest file with it
             * specific format.
             */
            while (fread(&buffer, sizeof(char) * 2, 1, fp) != 0) {

                if (strcmp(from, unx) == 0) { // Case text converted from unix
                    if (strcmp(to, win) == 0) { // Case linux to windows ['\n' --> '\r\n']
                        if (buffer[i] == 0x000a) {
                            buffer[i] = 0x000d;
                            fwrite(buffer, sizeof(char) * 2, 1, fp2);
                            buffer[i] = 0x000a;
                        }
                    } else { // Case linux to mac ['\n' --> '\r']
                        if (buffer[i] == 0x000a) {
                            buffer[i] = 0x000d;
                        }
                    }
                }
                else if (strcmp(from, win) == 0) { // Case text converted from windows
                    if (strcmp(to, unx) == 0) { // Case windows to unix ['\r\n' --> '\n']
                        if (buffer[i] == 0x000d) {
                            fread(&buffer, sizeof(char) * 2, 1, fp);
                        }
                    } else { // Case windows to mac ['\r\n' --> '\r']
                        if (buffer[i] == 0x000a) {
                            if (fread(&buffer, sizeof(char) * 2, 1, fp) == 0) {
                                fclose(fp2);
                            }
                        }
                    }
                } else { // Case text converted from mac
                    if (strcmp(to, unx) == 0) { // Case mac to unix ['\r' --> '\n']
                        if (buffer[i] == 0x000d) {
                            buffer[i] = 0x000a;
                        }
                    } else { // Case mac to windows ['\r' --> '\r\n']
                        if (buffer[i] == 0x000d) {
                            fwrite(buffer, sizeof(char) * 2, 1, fp2);
                            buffer[i] = 0x000a;
                        }
                    }
                }
                if (fp2) { // If file still open -> write data
                    fwrite(buffer, sizeof(char) * 2, 1, fp2);
                }
            }
            // Close files
            fclose(fp);
            fclose(fp2);
        } else { // Couldn't open source file, exit program.
            exit(0);
        }
    }
}
/*
 * Option 3: This option similar to option 2 but it have another flag to
 * decide whether the dest file should written by big or little endian format.
 * It have option of 'swap' flag that the bytes order should swap for every single
 * character and 'keep' swap that keep the file in the same format.
 */
void option_3(char* src_file, char* dst_file, char* from, char* to, char* flag) {
    // Case swap flag
    if (strcmp(flag, swap) == 0) {
        int i, j;
        char buffer[2]; // Buffer to store data
        FILE *fp;       // source file pointer
        FILE *fp2;      // destination file pointer
        // Trying to open source file, if not exist - exit.
        if ((fp = fopen(src_file, "rb")) != NULL) {
            fp2 = fopen(dst_file, "wb");
            char temp;
            // Reading the first 2 bytes to decide whether is little or big endian
            fread(&buffer, sizeof(char) * 2, 1, fp);
            if (buffer[0] == bom_1 && buffer[1] == bom_2) {
                i = 0;
                j = 1;
            } else {
                i = 1;
                j = 0;
            }
            // Write the bom to the destination file, while swapping the bytes order
            temp = buffer[i];
            buffer[i] = buffer[j];
            buffer[j] = temp;
            fwrite(buffer, sizeof(char) * 2, 1, fp2);
            /*
             * read the src file content and write it in the dest file with it
             * specific format, while swapping the bytes.
             */
            while (fread(&buffer, sizeof(char) * 2, 1, fp) != 0) {

                if (strcmp(from, unx) == 0) { // Case text converted from unix
                    if (strcmp(from, to) == 0) { // unix to unix
                    } else if (strcmp(to, win) == 0) { // Case linux to windows ['\n' --> '\r\n']
                        if (buffer[i] == 0x000a) {
                            temp = 0x000d;
                            buffer[i] = buffer[j];
                            buffer[j] = temp;
                            fwrite(buffer, sizeof(char) * 2, 1, fp2);
                            buffer[i] = 0x000a;
                            buffer[j] = 0;
                        }
                    } else { // Case linux to mac ['\n' --> '\r']
                        if (buffer[i] == 0x000a) {
                            buffer[i] = 0x000d;
                        }
                    }
                }
                else if (strcmp(from, win) == 0) { // Case text converted from windows
                    if (strcmp(from, to) == 0) { // win to win
                    } else if (strcmp(to, unx) == 0) { // Case windows to unix ['\r\n' --> '\n']
                        if (buffer[i] == 0x000d) {
                            fread(&buffer, sizeof(char) * 2, 1, fp);
                        }
                    } else { // Case windows to mac ['\r\n' --> '\r']
                        if (buffer[i] == 0x000a) {
                            if (fread(&buffer, sizeof(char) * 2, 1, fp) == 0) {
                                fclose(fp2);
                            }
                        }
                    }
                } else { // Case text converted from mac
                    if (strcmp(from, to) == 0) { // mac to mac
                    } else if (strcmp(to, unx) == 0) { // Case mac to unix ['\r' --> '\n']
                        if (buffer[i] == 0x000d) {
                            buffer[i] = 0x000a;
                        }
                    } else { // Case mac to windows ['\r' --> '\r\n']
                        if (buffer[i] == 0x000d) {
                            temp = buffer[i];
                            buffer[i] = buffer[j];
                            buffer[j] = temp;
                            fwrite(buffer, sizeof(char) * 2, 1, fp2);
                            buffer[i] = 0x000a;
                            buffer[j] = 0;
                        }
                    }
                }
                temp = buffer[i];
                buffer[i] = buffer[j];
                buffer[j] = temp;
                if (fp2) { // If file still open -> write data
                    fwrite(buffer, sizeof(char) * 2, 1, fp2);
                }
            }
            // Close files
            fclose(fp);
            fclose(fp2);
        } else { // Couldn't open source file, exit program.
            exit(0);
        }
    } // If keep flag is on, calling option 2.
    if (strcmp(flag, keep) == 0) {
        option_2(src_file, dst_file, from, to);
    }
}
/*
 * Main function to run the program.
 * The main gets command line arguments.
 */
int main(int argc, char **argv) {
    /*
     * Switch case to mapping the arguments.
     * If argc contain 3 args (program, src file, dest file) case option 1,
     * if argc contain 5 args (program, src file, dst file, format from, format to)
     * its run option 2, and if 6 arguments like option 2 extra keep\swap flag its
     * run option 3. otherwise bad arguments program exit.
     */
    switch (argc) {
        case 3:
            option_1(argv[1], argv[2]);
            break;
        case 5:
            // Checking whether from or to flags given keep\swap flag instead.
            if (strcmp(argv[3], swap) == 0 || strcmp(argv[3], keep) == 0 ||
            strcmp(argv[4], swap) == 0 || strcmp(argv[4], keep) == 0) {
                exit(0);
            } else {
                option_2(argv[1], argv[2], argv[3], argv[4]);
            }
            break;
        case 6:
            option_3(argv[1], argv[2], argv[3], argv[4], argv[5]);
            break;
        default:
            exit(0);
    }
}
