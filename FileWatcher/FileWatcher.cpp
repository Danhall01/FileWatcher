#include <iostream>
#include <string>
#include <sys/stat.h>


typedef struct file
{
    const char* filepath;
    void (*callable)(time_t);
    time_t lastModified;
} file_s;

static time_t FileCreationTime(const char* f)
{
    struct stat attr;
    stat(f, &attr);
    return attr.st_mtime;
}
static void FileWatchPoll(file_s* files, size_t fcount)
{
    // Check every file for updates
    for (int32_t i = 0; i < fcount; ++i)
    {
        time_t parse_t = FileCreationTime(files[i].filepath);
        if (parse_t == 0) {
            printf("Failed to get modified time for %s\n", files[i].filepath);
            continue;
        }

        if (files[i].lastModified != parse_t) {
            files[i].lastModified = parse_t;
            files[i].callable(parse_t);
        }
    }
}

void PrintTime(time_t time)
{
    char buffer[256];
    ctime_s(buffer, 256, &time);
    std::cout << "Modified at: " << buffer;
}

int main(void)
{
    file_s f[] = {
        {
            "C:/Users/wrait/source/repos/FileWatcher/FileWatcher/test.hlsl",
            PrintTime,
            0
        },
        {
            "C:/Users/wrait/source/repos/FileWatcher/FileWatcher/textfile.txt",
            PrintTime,
            0
        }
    };

    while (true)
    {
#ifdef _DEBUG
        FileWatchPoll(f, sizeof(f) / sizeof(*f));
#endif
    }
}
