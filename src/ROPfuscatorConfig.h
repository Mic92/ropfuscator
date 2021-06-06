#ifndef ROPFUSCATORCONFIG_H
#define ROPFUSCATORCONFIG_H

#include "OpaqueConstruct.h"
#include <map>
#include <string>
#include <vector>

namespace ropf {

// =========================
// CONFIGURATION FILE STRINGS
// =========================

#define CONFIG_GENERAL_SECTION   "general"
#define CONFIG_FUNCTIONS_SECTION "functions"
#define CONFIG_FUNCTIONS_DEFAULT "default"

// =========================
// General configuration options
// =========================

#define CONFIG_OBFUSCATION_ENABLED "obfuscation_enabled"
#define CONFIG_SEARCH_SEGMENT      "search_segment_for_gadget"
#define CONFIG_AVOID_MULTIVER      "avoid_multiversion_symbol"
#define CONFIG_CUSTOM_LIB_PATH     "custom_library_path"
#define CONFIG_LIB_SHA1            "library_hash_sha1"
#define CONFIG_LINKED_LIBS         "linked_libraries"
#define CONFIG_SHOW_PROGRESS       "show_progress"
#define CONFIG_PRINT_INSTR_STAT    "print_instr_stat"
#define CONFIG_USE_CHAIN_LABEL     "use_chain_label"

// =========================
// Functions-specific options
// =========================

#define CONFIG_FUNCTION_NAME "name"

// opaque predicates
#define CONFIG_OPAQUE_PREDICATED_ENABLED   "opaque_predicates_enabled"
#define CONFIG_OPAQUE_PREDICATES_ALGORITHM "opaque_predicates_algorithm"
#define CONFIG_OPAQUE_PREDICATES_INPUT_ALGORITHM                               \
  "opaque_predicates_input_algorithm"
#define CONFIG_CONTEXTUAL_OPAQUE_PREDICATES_ENABLED                            \
  "contextual_opaque_predicates_enabled"

// opaque constants
#define CONFIG_OPAQUE_GADGET_ADDRESSES_ENABLED "opaque_gadget_addresses_enabled"
#define CONFIG_OPAQUE_GADGET_ADDRESSES_PERCENTAGE                              \
  "gadget_addresses_obfuscation_percentage"
#define CONFIG_OPAQUE_IMMEDIATE_OPERANDS_ENABLED                               \
  "opaque_immediate_operands_enabled"
#define CONFIG_OPAQUE_BRANCH_TARGETS_ENABLED "opaque_branch_targets_enabled"
#define CONFIG_OPAQUE_STACK_VALUES_ENABLED   "opaque_saved_stack_values_enabled"

// branch divergence
#define CONFIG_BRANCH_DIVERGENCE_ENABLED      "branch_divergence_enabled"
#define CONFIG_BRANCH_DIVERGENCE_MAX_BRANCHES "branch_divergence_max_branches"
#define CONFIG_BRANCH_DIVERGENCE_ALGORITHM    "branch_divergence_algorithm"

// steganography
#define CONFIG_STEGANOGRAPHY_ENABLED "opaque_stegano_enabled"

//===========================

/// obfuscation configuration parameter for each function
struct ObfuscationParameter {
  /// true if obfuscation is enabled for this function
  bool obfuscationEnabled;
  /// true if opaque construct is enabled for this function
  bool opaquePredicatesEnabled;
  /// true if obfuscation of immediate operand is enabled for this function
  /// (only effective if opaquePredicatesEnabled == true)
  bool opaqueImmediateOperandsEnabled;
  /// true if contextual opaque predicate is enabled
  bool contextualOpaquePredicatesEnabled;
  /// true if obfuscation of branch address is enabled for this function
  /// (only effective if opaquePredicatesEnabled == true)
  bool opaqueBranchTargetsEnabled;
  /// true if save dummy constants
  bool opaqueSavedStackValuesEnabled;
  /// true if instruction steganography into opaque predicates enabled
  bool opaqueSteganoEnabled;
  /// true if branch divergence is enabled for this function
  bool branchDivergenceEnabled;
  /// true if gadget addresses should be obfuscated with opaque constants
  bool opaqueGadgetAddressesEnabled;
  /// percentage of total addresses to obfuscate for this function
  unsigned int gadgetAddressesObfuscationPercentage;
  /// maximum number of branches in branch divergence
  unsigned int branchDivergenceMaxBranches;
  /// opaque constant algorithm for this function
  std::string opaqueConstantsAlgorithm;
  /// opaque predicate input generation algorithm for this function
  std::string opaqueInputGenAlgorithm;
  /// branch divergence algorithm for this function
  std::string branchDivergenceAlgorithm;

  ObfuscationParameter()
      : obfuscationEnabled(true), opaquePredicatesEnabled(false),
        opaqueImmediateOperandsEnabled(true),
        contextualOpaquePredicatesEnabled(true),
        opaqueBranchTargetsEnabled(true), opaqueSavedStackValuesEnabled(true),
        opaqueSteganoEnabled(false), branchDivergenceEnabled(false),
        opaqueGadgetAddressesEnabled(true),
        gadgetAddressesObfuscationPercentage(100),
        branchDivergenceMaxBranches(32),
        opaqueConstantsAlgorithm(OPAQUE_CONSTANT_ALGORITHM_MOV),
        opaqueInputGenAlgorithm(OPAQUE_RANDOM_ALGORITHM_ADDREG),
        branchDivergenceAlgorithm(OPAQUE_BRANCH_ALGORITHM_ADDREG_MOV) {}
};

/// obfuscation configuration for the entire compilation unit
struct GlobalConfig {
  // [BinaryAutopsy] library path where the gadgets are extracted
  std::string libraryPath;
  // [BinaryAutopsy] expected library sha1 hash where the gadgets are extracted
  // If set, SHA1 hash is checked and stop obfuscation if it does not match
  std::string librarySHA1;
  // [BinaryAutopsy] other library paths linked at run-time
  // If set, the symbol names in these libraries are put in avoid-list in gadget
  std::vector<std::string> linkedLibraries;
  // true if obfuscation is enabled, false if obfuscation is disabled globally
  bool obfuscationEnabled;
  // [BinaryAutopsy] If set to true, find gadget in code segment instead of code
  // section (which will find more gadgets since code segment is wider)
  bool searchSegmentForGadget;
  // [BinaryAutopsy] If set to true, symbols which have multiple versions are
  // not used; if set to false, only one version of those symbols is used. (angr
  // will not work correctly if this is set to false)
  bool avoidMultiversionSymbol;
  // show obfuscation progress
  bool showProgress;
  // print instruction obfuscated status
  bool printInstrStat;
  // use chain label
  bool useChainLabel;

  GlobalConfig()
      : libraryPath(), librarySHA1(), linkedLibraries(),
        obfuscationEnabled(true), searchSegmentForGadget(true),
        avoidMultiversionSymbol(false), showProgress(false),
        printInstrStat(false), useChainLabel(false) {}
};

struct ROPfuscatorConfig {
  ObfuscationParameter                        defaultParameter;
  GlobalConfig                                globalConfig;
  std::map<std::string, ObfuscationParameter> functionsParameter;

  ObfuscationParameter getParameter(const std::string &funcname) const;

  void loadFromFile(const std::string &filename);
};

} // namespace ropf

#endif
