#include <exception>
#include <string>
#include <iostream>
#include "virtualFS.hpp"

int main() {
  std::unique_ptr<Folder> root(new Folder("root"));
  std::unique_ptr<FSNode> buffer;
  Folder *curDir = root.get(); 

  while (true) {
    std::cout << ">>> ";
    std::string cmd;
    std::cin >> cmd;

    if (cmd == "add") {
      std::string name;
      std::cin >> name; 
      
      std::string path;
      std::cin >> path;

      try {
        curDir->Add(new File(name, path));
      }
      catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
      }
    }

    else if (cmd == "ls") {
      auto arr = curDir->GetChildren();
      for (int i = 0; i < arr->GetSize(); i++) {
        std::cout << arr->At(i)->GetName() << " ";
      }
      std::cout << std::endl;
    }

    else if (cmd == "mkdir") {
      std::string name;
      std::cin >> name;

      try {
        curDir->Add(new Folder(name));
      }
      catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
      }
    }

    else if (cmd == "cd") {
      std::string name;
      std::cin >> name;
      Folder* folder = dynamic_cast<Folder*>(curDir->GetNodeByName(name));

      if (name == ".." && curDir->ParentFolder()) {
        curDir = curDir->ParentFolder();
      } 
      else if (folder) {
        curDir = folder;
      }
      else {
        std::cout << "\"" << name << "\" - isn't a folder" << std::endl;
      }
    }

    else if (cmd == "paste") {
      if (buffer) { 
        curDir->Add(buffer.release());
      }
      else {
        std::cout << "nothing to paste" << std::endl;
      }
    }

    else if (cmd == "rename")
    {
      std::string oldName;
      std::string newName;

      std::cin >> oldName >> newName;
      FSNode *fsnode = curDir->GetNodeByName(oldName);
      if (!fsnode) {
        std::cout << "no such file or directory" << std::endl;
      }
      else {
        try {
          fsnode->SetName(newName);
        }
        catch (const std::exception &e) {
          std::cerr << e.what() << std::endl;
        }
      }
    }

    else if (cmd == "cut") {
      std::string name;
      std::cin >> name;

      FSNode* fsnode = curDir->GetNodeByName(name);
      if (!fsnode) {
        std::cout << "no such file or directory" << std::endl;
      }
      else {
        buffer = fsnode->Move();
      }
    }

    else if (cmd == "exec") {
      std::string fileName;
      std::cin >> fileName;

      std::string extCmd;
      std::getline (std::cin, extCmd);

      File *file = dynamic_cast<File*>(curDir->GetNodeByName(fileName));   
      if (!file) {
        std::cout << "no such file or directory" << std::endl;
        continue;
      }

      int pos = 0;
      while ((pos = extCmd.find("%", pos)) != std::string::npos) {
        extCmd.erase(pos, 1);
        extCmd.insert(pos, file->GetRealPath());
        pos += file->GetRealPath().size();
      }

      system(extCmd.c_str());
    }

    else if (cmd == "exit") {
      return 0;
    }

    else {
      std::cout << "unknown command" << std::endl;
    }
  }
}
