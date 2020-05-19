//
// Basic instrumentation profiler by Cherno

// Usage: include this header file somewhere in your code (eg. precompiled header), and then use like:
//
// Instrumentor::get().beginSession("Session Name");        // Begin session
// {
//     InstrumentationTimer timer("Profiled Scope Name");   // Place code like this in scopes you'd like to include in
//     profiling
//     // Code
// }
// Instrumentor::get().endSession();                        // End Session
//
// You will probably want to macro-fy this, to switch on/off easily and use things like __FUNCSIG__ for the profile
// name.
//
#pragma once

#include <algorithm>
#include <chrono>
#include <fstream>
#include <string>
#include <thread>

namespace Hazel {

struct ProfileResult {
    std::string name;
    long long start, end;
    uint32_t thread_id;
};

struct InstrumentationSession {
    std::string name;
};

class Instrumentor {
private:
    InstrumentationSession* current_session_{nullptr};
    std::ofstream output_stream_;
    int profile_count_{0};

public:
    void beginSession(const std::string& name, const std::string& filepath = "results.json")
    {
        output_stream_.open(filepath);
        writeHeader();
        current_session_ = new InstrumentationSession{name};
    }

    void endSession()
    {
        writeFooter();
        output_stream_.close();
        delete current_session_;
        current_session_ = nullptr;
        profile_count_ = 0;
    }

    void writeProfile(const ProfileResult& result)
    {
        if (profile_count_++ > 0)
            output_stream_ << ",";

        std::string name = result.name;
        std::replace(name.begin(), name.end(), '"', '\'');

        output_stream_ << "{";
        output_stream_ << "\"cat\":\"function\",";
        output_stream_ << "\"dur\":" << (result.end - result.start) << ',';
        output_stream_ << "\"name\":\"" << name << "\",";
        output_stream_ << "\"ph\":\"X\",";
        output_stream_ << "\"pid\":0,";
        output_stream_ << "\"tid\":" << result.thread_id << ",";
        output_stream_ << "\"ts\":" << result.start;
        output_stream_ << "}";

        output_stream_.flush();
    }

    void writeHeader()
    {
        output_stream_ << "{\"otherData\": {},\"traceEvents\":[";
        output_stream_.flush();
    }

    void writeFooter()
    {
        output_stream_ << "]}";
        output_stream_.flush();
    }

    static Instrumentor& get()
    {
        static Instrumentor instance;
        return instance;
    }
};

class InstrumentationTimer {
public:
    InstrumentationTimer(const char* name) : name_(name), stopped_(false)
    {
        start_timepoint_ = std::chrono::high_resolution_clock::now();
    }

    ~InstrumentationTimer()
    {
        if (!stopped_)
            stop();
    }

    void stop()
    {
        auto endTimepoint = std::chrono::high_resolution_clock::now();

        long long start =
            std::chrono::time_point_cast<std::chrono::microseconds>(start_timepoint_).time_since_epoch().count();
        long long end =
            std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

        uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
        Instrumentor::get().writeProfile({name_, start, end, threadID});

        stopped_ = true;
    }

private:
    const char* name_;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_timepoint_;
    bool stopped_;
};

}  // namespace Hazel

#if HZ_ENABLE_INSTRUMENTATION
#define HZ_CONCATENATE_IMPL(s1, s2) s1##s2
#define HZ_CONCATENATE(s1, s2) HZ_CONCATENATE_IMPL(s1, s2)

#define HZ_PROFILE_BEGIN_SESSION(nAME, fILEPATH) ::Hazel::Instrumentor::get().beginSession(nAME, fILEPATH)
#define HZ_PROFILE_END_SESSION() ::Hazel::Instrumentor::get().endSession()
#define HZ_PROFILE_SCOPE(nAME) \
    ::Hazel::InstrumentationTimer HZ_CONCATENATE(timer, __LINE__) { nAME }
#define HZ_PROFILE_FUNCTION() HZ_PROFILE_SCOPE(__FUNCSIG__)
#else
#define HZ_PROFILE_BEGIN_SESSION(nAME, fILEPATH)
#define HZ_PROFILE_END_SESSION()
#define HZ_PROFILE_SCOPE(nAME)
#define HZ_PROFILE_FUNCTION()
#endif  // HZ_ENABLE_INSTRUMENTATION
