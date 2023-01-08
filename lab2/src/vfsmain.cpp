#include <unistd.h>

#include <algorithm>
#include <climits>
#include <exception>
#include <iostream>
#include <memory>
#include <string>

#include "colormod.h"
#include "dictionary/indexing.hpp"
#include "virtualFS.hpp"

struct Key
{
  std::string name = "";
  std::string path = "";
};

using DictPtr = std::shared_ptr<IndexedDict<Key, FSNode *>>;

int cmpPaths(const Key &a, const Key &b) { return a.path.compare(b.path); }
int cmpNames(const Key &a, const Key &b) { return a.name.compare(b.name); }

Key getKey(const FSNode *node)
{
  Key key;
  key.name = node->GetName();
  key.path = node->GetPath();
  return key;
}

FSNode *add(Folder *folder, FSNode *node, DictPtr &dict)
{
  folder->Add(node);
  dict->Add(node);
  return node;
}

std::unique_ptr<FSNode> move(FSNode *node, DictPtr &dict)
{
  dict->Remove(getKey(node));
  return std::move(node->Move());
}

std::string getNameFromPath(const std::string &path) { return path.substr(path.rfind("/") + 1); }

Folder *changeDir(const Folder *curDir, std::string str, const DictPtr &dict)
{
  Key key;
  Nullable<FSNode *> found;

  if (str[str.size() - 1] == '/')
  {
    str.pop_back();
  }

  if (str == "..")
  {
    return curDir->ParentFolder();
  }

  if (str[0] == '/')
  {
    key.path = str;
    key.name = getNameFromPath(str);
    found = dict->Get(key);
  }
  else if (str.find("/") != std::string::npos)
  {
    key.path = curDir->GetPath() + "/" + str;
    key.name = getNameFromPath(str);
    found = dict->Get(key);
  }
  else
  {
    return dynamic_cast<Folder *>(curDir->GetNodeByName(str));
  }

  if (!found.IsNull())
  {
    return dynamic_cast<Folder *>(found.GetValue());
  }

  return nullptr;

}

DictPtr searchByName(const Folder *folder, const DictPtr &dict, std::string name)
{
  Key min;
  Key max;

  char lastChar[2] = {CHAR_MAX, 0};

  min.path = folder->GetPath() + "/";
  max.path = min.path + std::string(lastChar);

  min.name = name;
  max.name = name + std::string(lastChar);

  return dict->GetInRange(min, max);
}

int main()
{
  std::unique_ptr<Folder> root(new Folder("root"));
  std::unique_ptr<FSNode> buffer;
  Folder *curDir = root.get();

  AttributesInfo<Key, FSNode *> attrs(
      {AttributeInfo<Key>("name", cmpNames), AttributeInfo<Key>("path", cmpPaths)}, getKey);
  auto dict = std::make_shared<IndexedDict<Key, FSNode *>>(attrs);
  dict->Add(root.get());

  while (true)
  {
    std::cout << curDir->GetPath() << ">";
    std::string cmd;
    std::cin >> cmd;

    if (cmd == "add")
    {
      std::string name;
      std::cin >> name;

      std::string path;
      std::cin >> path;

      if (access(path.c_str(), F_OK) == -1)
      {
        std::cerr << "file \"" + path + "\" cannot be opened" << std::endl;
      }
      else
      {
        try
        {
          add(curDir, new File(name, path), dict);
        }
        catch (const std::exception &e)
        {
          std::cerr << e.what() << std::endl;
        }
      }
    }

    else if (cmd == "ls")
    {
      auto arr = curDir->GetChildren();
      for (int i = 0; i < arr->GetSize(); i++)
      {
        Color::Modifier green(Color::Code::FG_GREEN);
        Color::Modifier defaultColor(Color::Code::FG_DEFAULT);

        if (dynamic_cast<Folder *>(arr->At(i)))
        {
          std::cout << green;
        }
        std::cout << arr->At(i)->GetName() << " " << defaultColor;
      }
      std::cout << std::endl;
    }

    else if (cmd == "mkdir")
    {
      std::string name;
      std::cin >> name;

      try
      {
        add(curDir, new Folder(name), dict);
      }
      catch (const std::exception &e)
      {
        std::cerr << e.what() << std::endl;
      }
    }

    else if (cmd == "cd")
    {
      std::string name;
      std::cin >> name;
      Folder *folder = changeDir(curDir, name, dict);

      if (folder)
      {
        curDir = folder;
      }
      else
      {
        std::cerr << "\"" << name << "\" - isn't a folder" << std::endl;
      }
    }

    else if (cmd == "paste")
    {
      if (buffer)
      {
        add(curDir, buffer.release(), dict);
      }
      else
      {
        std::cerr << "nothing to paste" << std::endl;
      }
    }

    else if (cmd == "rename")
    {
      std::string oldName;
      std::string newName;

      std::cin >> oldName >> newName;
      FSNode *fsnode = curDir->GetNodeByName(oldName);
      if (!fsnode)
      {
        std::cerr << "no such file or directory" << std::endl;
      }
      else
      {
        try
        {
          fsnode->SetName(newName);
        }
        catch (const std::exception &e)
        {
          std::cerr << e.what() << std::endl;
        }
      }
    }

    else if (cmd == "cut")
    {
      std::string name;
      std::cin >> name;

      FSNode *fsnode = curDir->GetNodeByName(name);
      if (!fsnode)
      {
        std::cerr << "no such file or directory" << std::endl;
      }
      else
      {
        buffer = move(fsnode, dict);
      }
    }

    else if (cmd == "exec")
    {
      std::string fileName;
      std::cin >> fileName;

      std::string extCmd;
      std::getline(std::cin, extCmd);

      File *file = dynamic_cast<File *>(curDir->GetNodeByName(fileName));
      if (!file)
      {
        std::cerr << "no such file or directory" << std::endl;
        continue;
      }

      int pos = 0;
      while ((pos = extCmd.find("%", pos)) != std::string::npos)
      {
        extCmd.erase(pos, 1);
        extCmd.insert(pos, file->GetRealPath());
        pos += file->GetRealPath().size();
      }

      system(extCmd.c_str());
    }

    else if (cmd == "search")
    {
      std::string path;
      std::cin >> path;

      auto results = searchByName(curDir, dict, path);
      std::cout << "results: " << std::endl;
      results->ForEach([](const FSNode *node)
                       { std::cout << "\t" << node->GetPath() << std::endl; });
    }

    else if (cmd == "exit")
    {
      return 0;
    }

    else
    {
      std::cerr << "unknown command" << std::endl;
    }
  }
}
