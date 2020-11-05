#ifndef BLACKJACK_OUTPUTMANAGER_H
#define BLACKJACK_OUTPUTMANAGER_H

#include <map>
#include <vector>
#include <string>
#include <memory>

struct OutputManager {
private:
    std::map<std::string, std::shared_ptr<struct ILogger>> subscribers;
public:
    OutputManager() = default;

    void subscribe(const std::string & name_listener, std::shared_ptr<struct ILogger> new_listener);
    void notify(const std::string & new_info);
    void unsubscribe(const std::string & listener);
    void drop();
};

struct ILogger {
protected:
    std::vector<std::string> buffer;
public:
    void update(std::string);
    virtual void output() = 0;
};

struct ConsoleLogger : public ILogger {
private:
    std::ostream& out;
public:
    explicit ConsoleLogger(std::ostream& os) : out(os){};
    void output() override;
};

#endif //BLACKJACK_OUTPUTMANAGER_H
