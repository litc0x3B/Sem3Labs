#include <map>
#include <unistd.h>
#include <functional>
#include <string>

void BubbleSort(int);

template<class T> using SortFunc = std::function<void(int)>;

template<class T> struct SortAlg {
    SortFunc<T> func;
    bool isSelected = false;
    std::string name;
};

int main(int argc, char *argv[])
{
    std::map<std::string, SortAlg<int>> algMap = {
        //{"bs", {BubbleSort, false, "BubbleSort"}}
    };

    int opt;
    char *avalue;

    while((opt = getopt(argc, argv, "a:b")) != -1)
    {
        switch (opt) 
        {
            case 'a': avalue = optarg; break;
        }
    }



}