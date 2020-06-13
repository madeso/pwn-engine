#define PWN_ENGINE_NO_AUTOLINK

#include <pwn/engine/key.h>
#include <pwn/sfml-config.h>
#include <SFML/Window/Event.hpp>
#include <pwn/core/stringutils.h>

#include <map>

namespace pwn
{
namespace engine
{
    namespace Key
    {
        const tchar* const
        ToString(const Code key)
        {
            switch (key)
            {
            case Unassigned: return "-";

#define KEY(k)                                                                 \
    case k: return #k
                KEY(A);
                KEY(B);
                KEY(C);
                KEY(D);
                KEY(E);
                KEY(F);
                KEY(G);
                KEY(H);
                KEY(I);
                KEY(J);
                KEY(K);
                KEY(L);
                KEY(M);
                KEY(N);
                KEY(O);
                KEY(P);
                KEY(Q);
                KEY(R);
                KEY(S);
                KEY(T);
                KEY(U);
                KEY(V);
                KEY(W);
                KEY(X);
                KEY(Y);
                KEY(Z);
                KEY(Num0);
                KEY(Num1);
                KEY(Num2);
                KEY(Num3);
                KEY(Num4);
                KEY(Num5);
                KEY(Num6);
                KEY(Num7);
                KEY(Num8);
                KEY(Num9);
                KEY(Escape);
                KEY(LControl);
                KEY(LShift);
                KEY(LAlt);
                KEY(LSystem);
                KEY(RControl);
                KEY(RShift);
                KEY(RAlt);
                KEY(RSystem);
                KEY(Menu);
                KEY(LBracket);
                KEY(RBracket);
                KEY(SemiColon);
                KEY(Comma);
                KEY(Period);
                KEY(Quote);
                KEY(Slash);
                KEY(BackSlash);
                KEY(Tilde);
                KEY(Equal);
                KEY(Dash);
                KEY(Space);
                KEY(Return);
                KEY(Back);
                KEY(Tab);
                KEY(PageUp);
                KEY(PageDown);
                KEY(End);
                KEY(Home);
                KEY(Insert);
                KEY(Delete);
                KEY(Add);
                KEY(Subtract);
                KEY(Multiply);
                KEY(Divide);
                KEY(Left);
                KEY(Right);
                KEY(Up);
                KEY(Down);
                KEY(Numpad0);
                KEY(Numpad1);
                KEY(Numpad2);
                KEY(Numpad3);
                KEY(Numpad4);
                KEY(Numpad5);
                KEY(Numpad6);
                KEY(Numpad7);
                KEY(Numpad8);
                KEY(Numpad9);
                KEY(F1);
                KEY(F2);
                KEY(F3);
                KEY(F4);
                KEY(F5);
                KEY(F6);
                KEY(F7);
                KEY(F8);
                KEY(F9);
                KEY(F10);
                KEY(F11);
                KEY(F12);
                KEY(F13);
                KEY(F14);
                KEY(F15);
                KEY(Pause);

                KEY(LMB);
                KEY(RMB);
                KEY(MMB);
                KEY(X1);
                KEY(X2);
#undef KEY
            default: return "?";
            }
        }

        namespace  // local
        {
            typedef std::map<pwn::string, Code> Map;
            Map
            BuildMap()
            {
                Map map;
                map.insert(std::make_pair<pwn::string, Code>("-", Unassigned));
#define KEY(k)                                                                 \
    map.insert(std::make_pair<pwn::string, Code>(pwn::core::ToLower(#k), k))
                KEY(A);
                KEY(B);
                KEY(C);
                KEY(D);
                KEY(E);
                KEY(F);
                KEY(G);
                KEY(H);
                KEY(I);
                KEY(J);
                KEY(K);
                KEY(L);
                KEY(M);
                KEY(N);
                KEY(O);
                KEY(P);
                KEY(Q);
                KEY(R);
                KEY(S);
                KEY(T);
                KEY(U);
                KEY(V);
                KEY(W);
                KEY(X);
                KEY(Y);
                KEY(Z);
                KEY(Num0);
                KEY(Num1);
                KEY(Num2);
                KEY(Num3);
                KEY(Num4);
                KEY(Num5);
                KEY(Num6);
                KEY(Num7);
                KEY(Num8);
                KEY(Num9);
                KEY(Escape);
                KEY(LControl);
                KEY(LShift);
                KEY(LAlt);
                KEY(LSystem);
                KEY(RControl);
                KEY(RShift);
                KEY(RAlt);
                KEY(RSystem);
                KEY(Menu);
                KEY(LBracket);
                KEY(RBracket);
                KEY(SemiColon);
                KEY(Comma);
                KEY(Period);
                KEY(Quote);
                KEY(Slash);
                KEY(BackSlash);
                KEY(Tilde);
                KEY(Equal);
                KEY(Dash);
                KEY(Space);
                KEY(Return);
                KEY(Back);
                KEY(Tab);
                KEY(PageUp);
                KEY(PageDown);
                KEY(End);
                KEY(Home);
                KEY(Insert);
                KEY(Delete);
                KEY(Add);
                KEY(Subtract);
                KEY(Multiply);
                KEY(Divide);
                KEY(Left);
                KEY(Right);
                KEY(Up);
                KEY(Down);
                KEY(Numpad0);
                KEY(Numpad1);
                KEY(Numpad2);
                KEY(Numpad3);
                KEY(Numpad4);
                KEY(Numpad5);
                KEY(Numpad6);
                KEY(Numpad7);
                KEY(Numpad8);
                KEY(Numpad9);
                KEY(F1);
                KEY(F2);
                KEY(F3);
                KEY(F4);
                KEY(F5);
                KEY(F6);
                KEY(F7);
                KEY(F8);
                KEY(F9);
                KEY(F10);
                KEY(F11);
                KEY(F12);
                KEY(F13);
                KEY(F14);
                KEY(F15);
                KEY(Pause);

                KEY(LMB);
                KEY(RMB);
                KEY(MMB);
                KEY(X1);
                KEY(X2);
#undef KEY
                return map;
            }

            const Map&
            KeyMap()
            {
                static const Map map = BuildMap();
                return map;
            }
        }

        const Code
        FromString(const tchar* const string)
        {
            namespace pc = pwn::core;
            Map::const_iterator res =
                    KeyMap().find(pc::ToLower(pc::Trim(string)));
            if (res == KeyMap().end())
            {
                return Undefined;
            }
            else
            {
                return res->second;
            }
        }
    }
}
}
