#pragma once
class CTTSGenerator
	{
	public:
		CTTSGenerator();
		~CTTSGenerator();

		bool GenerateSound(CString const & a_Text, CString const & a_FileName);
	};

