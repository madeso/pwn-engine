#ifndef PWN_ENGINE_KEYS
#define PWN_ENGINE_KEYS

#include <pwn/string.h>

namespace pwn
{
	namespace engine
	{
		namespace Key
		{
			enum Code
			{
			   Undefined
			   , Unassigned

			   , A = 'a'
			         , B = 'b'
			               , C = 'c'
			                     , D = 'd'
			                           , E = 'e'
			                                 , F = 'f'
			                                       , G = 'g'
			                                             , H = 'h'
			                                                   , I = 'i'
			                                                         , J = 'j'
			                                                               , K = 'k'
			                                                                     , L = 'l'
			                                                                           , M = 'm'
			                                                                                 , N = 'n'
			                                                                                       , O = 'o'
			                                                                                             , P = 'p'
			                                                                                                   , Q = 'q'
			                                                                                                         , R = 'r'
			                                                                                                               , S = 's'
			                                                                                                                     , T = 't'
			                                                                                                                           , U = 'u'
			                                                                                                                                 , V = 'v'
			                                                                                                                                       , W = 'w'
			                                                                                                                                             , X = 'x'
			                                                                                                                                                   , Y = 'y'
			                                                                                                                                                         , Z = 'z'
			                                                                                                                                                               , Num0 = '0'
			                                                                                                                                                                     , Num1 = '1'
			                                                                                                                                                                           , Num2 = '2'
			                                                                                                                                                                                 , Num3 = '3'
			                                                                                                                                                                                       , Num4 = '4'
			                                                                                                                                                                                             , Num5 = '5'
			                                                                                                                                                                                                   , Num6 = '6'
			                                                                                                                                                                                                         , Num7 = '7'
			                                                                                                                                                                                                               , Num8 = '8'
			                                                                                                                                                                                                                     , Num9 = '9'
			                                                                                                                                                                                                                           , Escape = 256
			                                                                                                                                                                                                                                 , LControl
			   , LShift
			   , LAlt
			   , LSystem
			   , RControl
			   , RShift
			   , RAlt
			   , RSystem
			   , Menu
			   , LBracket
			   , RBracket
			   , SemiColon
			   , Comma
			   , Period
			   , Quote
			   , Slash
			   , BackSlash
			   , Tilde
			   , Equal
			   , Dash
			   , Space
			   , Return
			   , Back
			   , Tab
			   , PageUp
			   , PageDown
			   , End
			   , Home
			   , Insert
			   , Delete
			   , Add
			   , Subtract
			   , Multiply
			   , Divide
			   , Left
			   , Right
			   , Up
			   , Down
			   , Numpad0
			   , Numpad1
			   , Numpad2
			   , Numpad3
			   , Numpad4
			   , Numpad5
			   , Numpad6
			   , Numpad7
			   , Numpad8
			   , Numpad9
			   , F1
			   , F2
			   , F3
			   , F4
			   , F5
			   , F6
			   , F7
			   , F8
			   , F9
			   , F10
			   , F11
			   , F12
			   , F13
			   , F14
			   , F15
			   , Pause

			   , LMB
			   , RMB
			   , MMB
			   , X1
			   , X2
			};

			// not localized, debug & io-only
			const tchar* const ToString(const Code key);
			const Code FromString(const tchar* const string);
		}
	}
}

#endif
