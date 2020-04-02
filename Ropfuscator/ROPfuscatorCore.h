// ==============================================================================
//   X86 ROPFUSCATOR
//   part of the ROPfuscator project
// ==============================================================================
// This module is simply the frontend of ROPfuscator for LLVM.
// It also provides statics about the processed functions.
//

#ifndef ROPFUSCATORCORE_H
#define ROPFUSCATORCORE_H

// To generate instruction coverage, define this
#define ROPFUSCATOR_INSTRUCTION_STAT

#ifdef ROPFUSCATOR_INSTRUCTION_STAT
#include <map>
#endif

#include "ROPfuscatorConfig.h"

// forward declaration
class BinaryAutopsy;
class ROPChain;
namespace llvm {
class MachineFunction;
class MachineBasicBlock;
class MachineInstr;
class Module;
class X86InstrInfo;
} // namespace llvm

class ROPfuscatorCore {
public:
  explicit ROPfuscatorCore(llvm::Module &module,
                           const ROPfuscatorConfig &config);
  ~ROPfuscatorCore();
  void obfuscateFunction(llvm::MachineFunction &MF);

private:
  ROPfuscatorConfig config;
  BinaryAutopsy *BA;
  const llvm::X86InstrInfo *TII;

#ifdef ROPFUSCATOR_INSTRUCTION_STAT
  struct ROPChainStatEntry;
  std::map<unsigned, ROPChainStatEntry> instr_stat;
#endif

  void insertROPChain(const ROPChain &chain, llvm::MachineBasicBlock &MBB,
                      llvm::MachineInstr &MI, int chainID,
                      const ObfuscationParameter &param);
};

#endif