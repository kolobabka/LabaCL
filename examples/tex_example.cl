tex_add_std_head ("report.tex", "LabaCl", "Frolov Daniil, Belov Vladislav");

tex_add_content ("report.tex");

tex_add_section ("report.tex", "What is it about?");

tex_add_text ("report.tex",
	      "LabaCL ~---~ interpreted language for semi-automatic lab works design. Based on
	       ParaCL (paralympic C-language). From programmers to physicists.
	       All sources you may find on \href{http://www.github.com/kolobabka/LabaCL}{LabaCL}");

//tex_add_text ("report.tex", "All thanks to Konstantin Vladimirov");

tex_add_section ("report.tex", "How does it work?");
tex_add_text ("report.tex", "We don't know");

tex_add_end ("report.tex");
tex_compile ("report.tex");
