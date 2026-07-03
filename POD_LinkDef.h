#ifdef __CLING__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

// clang-format off
// `POD_Event.hpp`
#pragma link C++ struct POD::Event+;
#pragma link C++ struct POD::MC::Event+;
// `POD_InjectedHdib.hpp`
#pragma link C++ struct POD::InjectedHdib+;
#pragma link C++ class  std::vector<POD::InjectedHdib>+;
// `POD_InjectedSexa.hpp`
#pragma link C++ struct POD::InjectedSexa+;
#pragma link C++ struct POD::Extended::InjectedSexa+;
#pragma link C++ struct POD::Linked::InjectedSexa+;
#pragma link C++ class  std::vector<POD::InjectedSexa>+;
#pragma link C++ class  std::vector<POD::Extended::InjectedSexa>+;
#pragma link C++ class  std::vector<POD::Linked::InjectedSexa>+;
// `POD_LambdaPair.hpp`
#pragma link C++ struct POD::LambdaPair+;
#pragma link C++ class  std::vector<POD::LambdaPair>+;
// `POD_McParticle.hpp`
#pragma link C++ struct POD::McParticle+;
#pragma link C++ struct POD::Extended::McParticle+;
#pragma link C++ class  std::vector<POD::McParticle>+;
#pragma link C++ class  std::vector<POD::Extended::McParticle>+;
// `POD_PreFoundLambda.hpp`
#pragma link C++ struct POD::PreFoundLambda+;
#pragma link C++ struct POD::Extended::PreFoundLambda+;
#pragma link C++ class  std::vector<POD::PreFoundLambda>+;
#pragma link C++ class  std::vector<POD::Extended::PreFoundLambda>+;
// `POD_Sexaquark.hpp`
#pragma link C++ struct POD::Sexaquark+;
#pragma link C++ class  std::vector<POD::Sexaquark>+;
// `POD_Track.hpp`
#pragma link C++ struct POD::Track+;
#pragma link C++ class  std::vector<POD::Track>+;
// `POD_V0.hpp`
#pragma link C++ struct POD::V0+;
#pragma link C++ class  std::vector<POD::V0>+;
// clang-format on
#endif
