#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pipe1[2]; // Pipe 1: Parent -> Child
    int pipe2[2]; // Pipe 2: Child -> Parent
    pid_t pid;

    // 1. Create two pipes using the pipe() system call
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }

    // 2. Create a child process using the fork() system call
    pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // ================= CHILD PROCESS =================
        close(pipe1[1]); // Close unused write end of pipe 1
        close(pipe2[0]); // Close unused read end of pipe 2

        char buffer[100];
        // 4. The child process reads the message received from the parent
        read(pipe1[0], buffer, sizeof(buffer));
        printf("Child received: %s\n", buffer);
        close(pipe1[0]);

        // 5. The child process sends the reply with VUID
        // Replace BC123456789 with your actual VUID
        char reply[] = "I am fine. My VUID is BC240401834";
        write(pipe2[1], reply, strlen(reply) + 1);
        close(pipe2[1]);

        exit(EXIT_SUCCESS);
    } else {
        // ================= PARENT PROCESS =================
        close(pipe1[0]); // Close unused read end of pipe 1
        close(pipe2[1]); // Close unused write end of pipe 2

        // 3. The parent process sends the message to the child process through the first pipe
        char message[] = "How are you? What's your VUID?";
        write(pipe1[1], message, strlen(message) + 1);
        close(pipe1[1]);

        // Wait for the child process to complete execution
        waitpid(pid, NULL, 0);

        // 6. The parent process reads and displays the reply received from the child
        char reply_buffer[100];
        read(pipe2[0], reply_buffer, sizeof(reply_buffer));
        printf("Parent received: %s\n", reply_buffer);
        close(pipe2[0]);
    }

    return 0;
}
```[cite: 10]

---

### **How to Run and Test**
1. Replace `BC240401834` inside the string `char reply[] = ...` with your actual VUID.
2. Compile and run it in your Linux environment (or OnlineGDB):
   ```bash
   gcc two_way_ipc.c -o two_way_ipc
   ./two_way_ipc
   ```[cite: 5, 10]
