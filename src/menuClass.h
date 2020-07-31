class Menu
{
public:
    int HideParagraph(int index)
    {
        if (Screens[index].find("$") > Screens[index].length())
        {
            Screens[index] = Screens[index] + std::string("$");
            return 0;
        }
        else
        {
            return -1;
        }
    }
    void ShowParagraph(int index)
    {
        if (Screens[index].find("$") > Screens[index].length())
        {
        }
        else
        {
            std::string* str = &Screens[index];
            str->erase(std::remove(str->begin(), str->end(), '$'), str->end());
            Screens[index] = *str;
        }
    }
    int SetActiveParagraph(int index)
    {
        for (int i = 0;  arraySize(Screens) > i; i++) 
        {
            if (isFind(Screens[i], "!"))
            {
                std::string* str = &Screens[i];
                str->erase(std::remove(str->begin(), str->end(), '!'), str->end());
                Screens[i] = *str;
            }
        }
        if (isFind(Screens[index], "$") == false) 
        {
           Screens[index] = std::string("!") + Screens[index];
           return 0;
        }
        else
        {
            return -1;
        }
        
    }
    int GetActiveParagrath()
    {
        for (int i = 0; arraySize(Screens) > i; i++)
        {
            if (isFind(Screens[i], "!"))
            {
                return i;
            }
        }
    }
    int GetIndex(std::string str) 
    {
        for (int i = 0; arraySize(Screens) > i; i++)
        {
            if (isFind(Screens[i], str))
            {
                return i;
            }
        }
    }
    std::string GetTextParagrath(int index)
    {
        return Screens[index];
    }
    int GetHomeParagrath() 
    {
        for (int i = 0; arraySize(Screens) > i; i++)
        {
            if (isFind(Screens[i], "Home"))
            {
                return i;
            }
        }
    }
    int GetStepsParagrath()
    {
        for (int i = 0; arraySize(Screens) > i; i++)
        {
            if (isFind(Screens[i], "Steps"))
            {
                return i;
            }
        }
    }
    int GetPulseParagrath()
    {
        for (int i = 0; arraySize(Screens) > i; i++)
        {
            if (isFind(Screens[i], "Pulse"))
            {
                return i;
            }
        }
    }
    int GetSettingsParagrath()
    {
        for (int i = 0; arraySize(Screens) > i; i++)
        {
            if (isFind(Screens[i], "Settings"))
            {
                return i;
            }
        }
    }
    int* GetAllIndexes()
    {
        int IndexesSize = arraySize(Screens);
        int* indexes = new int[IndexesSize];
        for (int i = 0; arraySize(Screens) > i; i++)
        {
            if (isFind(Screens[i], "Settings"))
            {
                indexes[i] = i;
            }
        }
        return indexes;
    }
private:
    std::string Screens[4] = {
                        "Home",
                        "Steps",
                        "Pulse",
                        "Settings"
    };

    template<typename T, size_t n>
    inline size_t arraySize(const T(&arr)[n])
    {
        return n;
    }

    bool isFind(std::string str, std::string subString)
    {
        if (str.find(subString) > str.length())
        {
            return false;
        }
        else
        {
            return true;
        }
    }
};
