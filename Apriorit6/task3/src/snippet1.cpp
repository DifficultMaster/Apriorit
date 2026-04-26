#include <cstdio>
#include <expected>
#include <string>
#include <iostream>
#include <optional>
#include <fstream>

// This snippet is copy-pasted code with comments explaining the problems
namespace apriorit6::task3::snippet3::before
{
    int ParseValue(const std::string& token) {
        // looks innocent, but throws on non-numeric input, purpose is completely defeated as there is no other logic except stoi
        return std::stoi(token);
    }

    void DoSomething(FILE* f) { // fstream is a modern RAII-safe alternative to the C-style FILE
        char buf[256];
        while (fgets(buf, sizeof(buf), f)) { // C legacy stuff, does not check errors, may easily infinite loop
            std::string line(buf);
            // strip newline
            line.erase(line.find_last_not_of("\n\r") + 1);

            int val = ParseValue(line); // throws if file has a bad line
            std::cout << "Value: " << val << "\n";
        }
    }

    void ProcessFile(const std::string& path)
    {
        FILE* f = fopen(path.c_str(), "r"); // this is a deprecated non-RAII safe C-style function, fstream should be used instead
        DoSomething(f); // does not check if file was opened successfully, may blindly pass NULL
        fclose(f); // ignores nodiscard warning, will crash if NULL, no exception safety upon throws, fstream should be used instead
    }
}

// This snippet is the same code, fixed to implement RAII correctly
namespace apriorit6::task3::snippet3::after
{
    // this stoi wrapper returns an optional instead of throwing, providing both RAII safety, a more flexible API and No-Throw guarantee
    std::optional<int> ParseValue(const std::string& token) 
	{
        try
        {
            return std::stoi(token);
        }
        catch (const std::invalid_argument&)
        {
            return std::nullopt;
        }
		catch (const std::out_of_range&)
        {
            return std::nullopt;
        }
    }

    // signature modified to match the RAII-safe fstream class
    void DoSomething(std::ifstream& in_file) 
	{
        std::string line; 

        // modern std::string syntax
        while (std::getline(in_file, line))
        {
            // only pops \r because getline already strips \n
            if (!line.empty() && line.back() == '\r')
            {
                line.pop_back();
            }

            // skip empty lines, checks after stripping line of both \n and \r
            if (line.empty()) continue;

            // display value if valid, display warning if invalid - no-throw guarantee
            if (std::optional<int> val = ParseValue(line); val.has_value())
            {
                std::cout << "Value: " << val.value() << "\n";
            }
            else
            {
				std::cerr << "Warning: Invalid line skipped: " << line << "\n";
            }
        }
    }

    // assuming this API handles (mostly) non-critical files, std::expected from C++23 is utilized to provide No-Throw guarantee and API flexibility
    // nodiscard is utilized to prevent silent ignore of unexpected
    [[nodiscard]] std::expected<void, std::string> ProcessFile(const std::string& path)
    {
		std::ifstream in_file(path); // fstream is a modern RAII-safe alternative to the C-style FILE

		if (!in_file.is_open()) // check if file was opened successfully
        {
			return std::unexpected("File not found: " + path);
        }

        DoSomething(in_file);

        return {};
       
        // when out of scope, in_file will gracefully self-destruct, providing RAII safety
    }
}