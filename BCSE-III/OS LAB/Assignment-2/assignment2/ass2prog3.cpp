#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <queue>
#include <random>
#include <sstream>

const int BUFFER_SIZE = 25; // Circular buffer size
int TOTAL = 0; // Shared variable to hold the total of consumed numbers

std::mutex mtx;
std::mutex total_mtx;  // Separate mutex for TOTAL
std::condition_variable not_full, not_empty;
std::queue<std::string> buffer; // Shared buffer

bool done_producing = false; // Flag to indicate all producers are done

// Function to generate a random number between 1 and 20
int generate_random_number() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 20);
    return dis(gen);
}

// Producer function
void producer(int id, int items_to_produce) {
    for (int i = 0; i < items_to_produce; ++i) {
        std::unique_lock<std::mutex> lock(mtx);
        not_full.wait(lock, [] { return buffer.size() < BUFFER_SIZE; });

        // Generate a random number and format it with the producer id
        int num = generate_random_number();
        std::stringstream item;
        item << "p" << id << (num < 10 ? "0" : "") << num;
        
        buffer.push(item.str());
        std::cout << "Producer " << id << " produced: " << item.str() << std::endl;

        lock.unlock();
        not_empty.notify_all();
    }
}

// Consumer function
void consumer(int id) {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        not_empty.wait(lock, [] { return !buffer.empty() || done_producing; });

        if (buffer.empty() && done_producing) break; // Exit if no more items and production done

        std::string item = buffer.front();
        buffer.pop();
        lock.unlock();
        not_full.notify_all();

        // Extract the number part from the item string
        int num = std::stoi(item.substr(2, 2));
        std::cout << "Consumer " << id << " consumed: " << item << ", extracted number: " << num << std::endl;

        // Update TOTAL atomically
        {
            std::lock_guard<std::mutex> total_lock(total_mtx);
            TOTAL += num;
        }
    }
}

int main() {
    int p, c;
    std::cout << "Enter the number of producers: ";
    std::cin >> p;
    std::cout << "Enter the number of consumers: ";
    std::cin >> c;

    int items_per_producer;
    std::cout << "Enter number of items each producer should produce: ";
    std::cin >> items_per_producer;

    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;

    // Create producer threads
    for (int i = 1; i <= p; ++i) {
        producers.emplace_back(producer, i, items_per_producer);
    }

    // Create consumer threads
    for (int i = 1; i <= c; ++i) {
        consumers.emplace_back(consumer, i);
    }

    // Wait for all producers to finish
    for (auto &prod : producers) {
        prod.join();
    }

    // Signal that production is done
    {
        std::lock_guard<std::mutex> lock(mtx);
        done_producing = true;
    }
    not_empty.notify_all(); // Notify all consumers that production is complete

    // Wait for all consumers to finish
    for (auto &cons : consumers) {
        cons.join();
    }

    // Print the final value of TOTAL
    std::cout << "Final TOTAL value: " << TOTAL << std::endl;
    return 0;
}

