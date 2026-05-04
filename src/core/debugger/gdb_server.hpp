#pragma once

#include "core/debugger/const.hpp"

namespace hydra::horizon::kernel {
class GuestThread;
} // namespace hydra::horizon::kernel

namespace hydra::debugger {

#if defined(PLATFORM_APPLE) || defined(PLATFORM_LINUX)
using socket_t = i32;
#else
using socket_t = u64;
#endif

class Thread;
class Debugger;

class GdbServer {
  public:
    GdbServer(Debugger& debugger_);
    ~GdbServer();

    void RegisterThread(Thread& thread);

    void NotifySupervisorPaused(horizon::kernel::GuestThread* thread,
                                Signal signal);
    void BreakpointHit(horizon::kernel::GuestThread* thread);

  private:
    Debugger& debugger;

    std::mutex mutex;

    socket_t server_socket;
    socket_t client_socket{static_cast<socket_t>(-1)};
    std::thread server_thread;
    std::atomic<bool> running{true};
    std::string receive_buffer;
    bool do_ack{true};

    horizon::kernel::GuestThread* crnt_thread;
    std::vector<vaddr_t> breakpoint_addresses;
    std::map<vaddr_t, u32> replaced_instructions;
    std::atomic<bool> breakpoint_hit{false};
    horizon::kernel::GuestThread* breakpoint_thread{nullptr};

    void CloseClientSocket();

    void ServerLoop();
    void Poll();

    void SendPacket(std::string_view data);
    void SendStatus(char status);

    void ProcessPackets();
    void HandleCommand(std::string_view command);

    // Commands
    void HandleVCont(std::string_view command);
    void HandleQuery(std::string_view command);
    void HandleSetActiveThread(std::string_view command);
    void HandleThreadStatus();
    void HandleRegRead(std::string_view command);
    void HandleMemRead(std::string_view command);
    void HandleInsertBreakpoint(std::string_view command);
    void HandleRemoveBreakpoint(std::string_view command);

    void HandleRcmd(std::string_view command);
    void HandleGetExecutables();

    // Helpers
    void SetNonBlocking(socket_t socket);
    std::string ReadReg(u32 id);
    std::string GetThreadStatus(horizon::kernel::GuestThread* thread,
                                Signal signal);
    std::string PageFromBuffer(std::string_view buffer, std::string_view page);

    void NotifySupervisorPausedImpl(horizon::kernel::GuestThread* thread,
                                    Signal signal);
    void NotifyMemoryChanged(Range<vaddr_t> mem_range);
};

} // namespace hydra::debugger
