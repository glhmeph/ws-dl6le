#include "widgets/JttyMessages.hpp"
#include <cstdlib>
#include <iostream>

extern "C" void jtty_macro_roundtrip(char const *, int, char *, int *);

static void check(bool condition, char const *message)
{
  if (!condition) { std::cerr << message << '\n'; std::exit(1); }
}

static void roundtrip(QString const &text, Jtty::NativeExchangeProfile profile)
{
  QByteArray const input = text.toLatin1().leftJustified(80, ' ');
  char output[80];
  int frames = 0;
  jtty_macro_roundtrip(input.constData(), static_cast<int>(profile), output, &frames);
  check(frames > 0 && frames <= 16, "Compound macro must encode successfully");
  // Match jtty_mdecode's conversion of the codec's space display marker.
  QString const decoded = QString::fromLatin1(output, 80).replace('~', ' ').trimmed();
  if (decoded != text) {
    std::cerr << "Expected: " << text.toStdString() << "\nDecoded: "
              << decoded.toStdString() << '\n';
  }
  check(decoded == text,
        "Codec must preserve the complete callsigns and macro text");
}

int main()
{
  using namespace Jtty;
  NativeMacroContext context {"DL6LE", "K1ABC", 12};
  for (int key = 1; key <= 8; ++key) {
    check(compileNativeMacro(nativeMacroTemplate(key), context).isNative(),
          "Standard calls must retain compact native encoding");
  }

  for (QString const &myCall : {QString {"TK/DL6LE"}, QString {"DL6LE/P"},
                               QString {" tk/dl6le "}}) {
    context.myCall = myCall;
    for (int key = 1; key <= 8; ++key) {
      auto const macro = compileNativeMacro(nativeMacroTemplate(key), context);
      bool const usesMyCall = key == 1 || key == 3 || key == 4;
      check(macro.status == (usesMyCall ? NativeMacroStatus::LiteralFallback
                                      : NativeMacroStatus::Native),
            "All default macros must work with a compound own call");
      if (usesMyCall) {
        check(macro.atoms.isEmpty(), "Fallback must not retain partial native atoms");
        check(macro.text == expandLiteralMacro(nativeMacroTemplate(key),
               NativeMacroContext {normalizedNativeCall(myCall), "K1ABC", 12}),
              "Own prefix or suffix must remain in the expanded macro");
        roundtrip(macro.text, context.exchangeProfile);
      }
    }
  }

  context.myCall = "TK/DL6LE";
  context.hisCall = "F/K1ABC/P";
  for (int key = 1; key <= 8; ++key) {
    auto const macro = compileNativeMacro(nativeMacroTemplate(key), context);
    check(macro.status == (key == 8 ? NativeMacroStatus::Native
                                    : NativeMacroStatus::LiteralFallback),
          "Compound DX and own calls must work in all call macros");
    roundtrip(macro.text, context.exchangeProfile);
  }
  for (int key : {2, 6, 8}) {
    auto const macro = compileNativeMacro(legacyNativeMacroTemplate(key), context);
    check(macro.status != NativeMacroStatus::InvalidRuntime, "Legacy macros must work");
    roundtrip(macro.text, context.exchangeProfile);
  }
  context.grid = "JN42";
  for (int key : {2, 6, 8}) {
    auto const macro = compileNativeMacro(nativeGridMacroTemplate(key), context);
    check(macro.status != NativeMacroStatus::InvalidRuntime, "Grid macros must work");
    roundtrip(macro.text, context.exchangeProfile);
  }

  context.exchangeProfile = NativeExchangeProfile::RttyRoundup;
  context.configuredExchange = "MA";
  auto macro = compileNativeMacro(nativeMacroTemplate(2), context);
  check(macro.text == "F/K1ABC/P 599 MA", "RTTY exchange must retain its meaning");
  roundtrip(macro.text, context.exchangeProfile);
  context.exchangeProfile = NativeExchangeProfile::FieldDay;
  context.configuredExchange = "1D EMA";
  macro = compileNativeMacro(nativeMacroTemplate(6), context);
  check(macro.text == "TU NOW F/K1ABC/P 1D EMA", "Field Day must not gain a 599 report");
  roundtrip(macro.text, context.exchangeProfile);
  context.configuredExchange = "INVALID";
  check(compileNativeMacro(nativeMacroTemplate(2), context).status
        == NativeMacroStatus::InvalidRuntime, "Fallback must still reject invalid exchanges");
  context.exchangeProfile = NativeExchangeProfile::None;
  context.serialNumber = -1;
  check(compileNativeMacro(nativeMacroTemplate(2), context).status
        == NativeMacroStatus::InvalidRuntime, "Fallback must still reject invalid serials");

  for (QString const &call : {QString {"TK//DL6LE"}, QString {"/DL6LE"},
                             QString {"DL6LE/"}, QString {"TK/INVALID"},
                             QString {"TK/DL6LE?"}, QString {"TK DL6LE"}}) {
    context.myCall = call;
    check(compileNativeMacro(nativeMacroTemplate(1), context).status
          == NativeMacroStatus::InvalidRuntime, "Malformed calls must still be rejected");
  }
  std::cout << "JTTY macro and codec roundtrip checks passed\n";
}
