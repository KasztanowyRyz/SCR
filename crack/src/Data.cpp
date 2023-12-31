#include "Data.hpp"

void Data::readFile(const std::string& hash_f) {
  clearVar();
  lines.clear();
  std::string tmp;

  std::fstream file;
  file.open(hash_f, std::ios::in);
  if (!file.good()) {
    throw std::invalid_argument("The file " + hash_f +
                                " was not opened properly or " + hash_f +
                                "does not exist!");
  }

  while (!file.eof()) {
    if (!getline(file, tmp)) continue;
    lines.emplace_back(tmp);
  }
  universalFilter();
}

void Data::showAll() {
  std::cout<<"All cracked passwords:" << std::endl;
  for(const auto& user : crackedPasswd) {
    std::cout << "\t -Password for " << user.second.getMail()
              << " is " << user.first << std::endl;
  }
}

void Data::clearVar() {
  this->hash2Hash.clear();
  this->hashPasswd.clear();
  this->numberOfLines = 0;
}

void Data::universalFilter() {
  clearVar();
  for (const auto & line : lines) {
    addHash(parser(line));
  }
}

std::vector<std::string> Data::parser(std::string line) {
  std::vector<std::string> parsedLine;
  std::string column;
  std::stringstream tmp(line);
  while (std::getline(tmp, column, '\t')) {
    parsedLine.emplace_back(std::move(column));
  }
  return parsedLine;
}

void Data::addHash(std::vector<std::string> parsed) {
  Hash hashedLine;
  if (parsed.size() == 4) {
    try {
      hashedLine.setOrdNo(stoul(parsed.at(0)));
    } catch (std::invalid_argument const &ex) {
      return;
    }
    try {
      hashedLine.setHashPasswd(parsed.at(1));
    } catch (std::invalid_argument const &ex) {
      return;
    }
    try {
      hashedLine.setMail(parsed.at(2));
    } catch (std::invalid_argument const &ex) {
      return;
    }
    try {
      hashedLine.setUserName(parsed.at(3));
    } catch (std::invalid_argument const &ex) {
      return;
    }

    hash2Hash[hashedLine.getHashPasswd()] = hashedLine;
    hashPasswd.emplace(hashedLine.getHashPasswd());
    numberOfLines++;
  }
}