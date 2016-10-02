%%
%% This is file `siugrad.cls',
%% (copied from the latex report.cls file)
%%
%% --------------------------------------------------------------
%%
%% This file has all formatting defined for typesetting theses,
%% research papers, and dissertations, for Department of
%% Mathematics grad students in a format approved by the
%% graduate school.
%%
%% Unless you really know what you're doing, you should not change
%% anything in this file.
%%
%% --------------------------------------------------------------
%%
%% Last updated: 06/26/03
%%               07/06/05
%%               07/03/06          (Uses separate title.tex file to create Title Page)
%%               07/24/06          (Merged with siugradx.cls NOCHAP sections)
%%               02/23/12   SIU-51 (Changed oddsidmargin, evensidemargin from 1.5 to 1.0 inches)
%%                          SIU-51 (Changed textwidth from 5.9in to 6.4 in)
%%
%% The original source files were:
%%
%% classes.dtx  (with options: `report')
%%
%% This is a generated file.
%%
%% Copyright 1993 1994 1995 1996 1997 1998 1999
%% The LaTeX3 Project and any individual authors listed elsewhere
%% in this file.
%%
%% This file is part of the LaTeX2e system.
%% ----------------------------------------
%%
%% It may be distributed under the terms of the LaTeX Project Public
%% License, as described in lppl.txt in the base LaTeX distribution.
%% Either version 1.0 or, at your option, any later version.
%% \CharacterTable
%%  {Upper-case    \A\B\C\D\E\F\G\H\I\J\K\L\M\N\O\P\Q\R\S\T\U\V\W\X\Y\Z
%%   Lower-case    \a\b\c\d\e\f\g\h\i\j\k\l\m\n\o\p\q\r\s\t\u\v\w\x\y\z
%%   Digits        \0\1\2\3\4\5\6\7\8\9
%%   Exclamation   \!     Double quote  \"     Hash (number) \#
%%   Dollar        \$     Percent       \%     Ampersand     \&
%%   Acute accent  \'     Left paren    \(     Right paren   \)
%%   Asterisk      \*     Plus          \+     Comma         \,
%%   Minus         \-     Point         \.     Solidus       \/
%%   Colon         \:     Semicolon     \;     Less than     \<
%%   Equals        \=     Greater than  \>     Question mark \?
%%   Commercial at \@     Left bracket  \[     Backslash     \\
%%   Right bracket \]     Circumflex    \^     Underscore    \_
%%   Grave accent  \`     Left brace    \{     Vertical bar  \|
%%   Right brace   \}     Tilde         \~}
\NeedsTeXFormat{LaTeX2e}[1995/12/01]
\ProvidesClass{siugrad51}	%%  SIU Update to replace line below
%\ProvidesClass{report}
              [1999/01/07 v1.4a
 Standard LaTeX document class]
\newcommand\@ptsize{}
\newif\if@restonecol
\newif\if@titlepage
%
%--------------------------------------------------
%% NOCHAP 07/24/06
\newif\ifmultiplechapters
\multiplechapterstrue
%--------------------------------------------------
%
\@titlepagetrue
\newif\if@openright
\if@compatibility\else
%--------------------------------------------------
%% NOCHAP 07/14/06
\DeclareOption{nochap}
   {\multiplechaptersfalse}
%--------------------------------------------------
\DeclareOption{a4paper}
   {\setlength\paperheight {297mm}%
    \setlength\paperwidth  {210mm}}
\DeclareOption{a5paper}
   {\setlength\paperheight {210mm}%
    \setlength\paperwidth  {148mm}}
\DeclareOption{b5paper}
   {\setlength\paperheight {250mm}%
    \setlength\paperwidth  {176mm}}
\DeclareOption{letterpaper}
   {\setlength\paperheight {11in}%
    \setlength\paperwidth  {8.5in}}
\DeclareOption{legalpaper}
   {\setlength\paperheight {14in}%
    \setlength\paperwidth  {8.5in}}
\DeclareOption{executivepaper}
   {\setlength\paperheight {10.5in}%
    \setlength\paperwidth  {7.25in}}
\DeclareOption{landscape}
   {\setlength\@tempdima   {\paperheight}%
    \setlength\paperheight {\paperwidth}%
    \setlength\paperwidth  {\@tempdima}}
\fi
\if@compatibility
  \renewcommand\@ptsize{0}
\else
\DeclareOption{10pt}{\renewcommand\@ptsize{0}}
\fi
\DeclareOption{11pt}{\renewcommand\@ptsize{1}}
\DeclareOption{12pt}{\renewcommand\@ptsize{2}}
\if@compatibility\else
\DeclareOption{oneside}{\@twosidefalse \@mparswitchfalse}
\fi
\DeclareOption{twoside}{\@twosidetrue  \@mparswitchtrue}
\DeclareOption{draft}{\setlength\overfullrule{5pt}}
\if@compatibility\else
\DeclareOption{final}{\setlength\overfullrule{0pt}}
\fi
\DeclareOption{titlepage}{\@titlepagetrue}
\if@compatibility\else
\DeclareOption{notitlepage}{\@titlepagefalse}
\fi
\if@compatibility
\else
\DeclareOption{openright}{\@openrighttrue}
\DeclareOption{openany}{\@openrightfalse}
\fi
\if@compatibility\else
\DeclareOption{onecolumn}{\@twocolumnfalse}
\fi
\DeclareOption{twocolumn}{\@twocolumntrue}
\DeclareOption{leqno}{\input{leqno.clo}}
\DeclareOption{fleqn}{\input{fleqn.clo}}
\DeclareOption{openbib}{%
  \AtEndOfPackage{%
   \renewcommand\@openbib@code{%
      \advance\leftmargin\bibindent
      \itemindent -\bibindent
      \listparindent \itemindent
      \parsep \z@
      }%
   \renewcommand\newblock{\par}}%
}
%%--------------------------------------------------------------
%%---  SIU:  Change 10pt in ExecuteOptions below to 12pt     ---
%%--------------------------------------------------------------
\ExecuteOptions{letterpaper,12pt,oneside,onecolumn,final,openany}
\ProcessOptions
\input{size1\@ptsize.clo}
\setlength\lineskip{1\p@}
\setlength\normallineskip{1\p@}
%%--------------------------------------------------------------
%%---  SIU:  Change baselinestretch{} below to               ---
%%---        baselinestretch{2} to double space everything.  ---
%%--------------------------------------------------------------
\renewcommand\baselinestretch{2}
\setlength\parskip{0\p@ \@plus \p@}
\@lowpenalty   51
\@medpenalty  151
\@highpenalty 301
\setcounter{topnumber}{2}
\renewcommand\topfraction{.7}
\setcounter{bottomnumber}{1}
\renewcommand\bottomfraction{.3}
\setcounter{totalnumber}{3}
\renewcommand\textfraction{.2}
\renewcommand\floatpagefraction{.5}
\setcounter{dbltopnumber}{2}
\renewcommand\dbltopfraction{.7}
\renewcommand\dblfloatpagefraction{.5}
\if@twoside
  \def\ps@headings{%
      \let\@oddfoot\@empty\let\@evenfoot\@empty
      \def\@evenhead{\thepage\hfil\slshape\leftmark}%
      \def\@oddhead{{\slshape\rightmark}\hfil\thepage}%
      \let\@mkboth\markboth
    \def\chaptermark##1{%
      \markboth {\MakeUppercase{%
        \ifnum \c@secnumdepth >\m@ne
            \@chapapp\ \thechapter. \ %
        \fi
        ##1}}{}}%
    \def\sectionmark##1{%
      \markright {\MakeUppercase{%
        \ifnum \c@secnumdepth >\z@
          \thesection. \ %
        \fi
        ##1}}}}
\else
  \def\ps@headings{%
    \let\@oddfoot\@empty
    \def\@oddhead{{\slshape\rightmark}\hfil\thepage}%
    \let\@mkboth\markboth
    \def\chaptermark##1{%
      \markright {\MakeUppercase{%
        \ifnum \c@secnumdepth >\m@ne
            \@chapapp\ \thechapter. \ %
        \fi
        ##1}}}}
\fi
%%--------------------------------------------------------------
%%---  SIU:  Override the title and some other definitions   ---
%%---        from latex.ltx.                                 ---
%%--------------------------------------------------------------
%
%
\def\ps@myheadings{%
    \let\@oddfoot\@empty\let\@evenfoot\@empty
    \def\@evenhead{\thepage\hfil\slshape\leftmark}%
    \def\@oddhead{{\slshape\rightmark}\hfil\thepage}%
    \let\@mkboth\@gobbletwo
    \let\chaptermark\@gobble
    \let\sectionmark\@gobble
    }
  \if@titlepage
  \newcommand\maketitle{\begin{titlepage}%
  \let\footnotesize\small
  \let\footnoterule\relax
  \let \footnote \thanks
  \end{titlepage}%
  \setcounter{footnote}{0}%
  \global\let\thanks\relax
  \global\let\maketitle\relax
  \global\let\@thanks\@empty
  \global\let\@author\@empty
  \global\let\@date\@empty
  \global\let\@title\@empty
  \global\let\title\relax
  \global\let\author\relax
  \global\let\date\relax
  \global\let\and\relax
}
\else
\newcommand\maketitle{\par
  \begingroup
    \renewcommand\thefootnote{\@fnsymbol\c@footnote}%
    \def\@makefnmark{\rlap{\@textsuperscript{\normalfont\@thefnmark}}}%
    \long\def\@makefntext##1{\parindent 1em\noindent
            \hb@xt@1.8em{%
                \hss\@textsuperscript{\normalfont\@thefnmark}}##1}%
    \if@twocolumn
      \ifnum \col@number=\@ne
        \@maketitle
      \else
        \twocolumn[\@maketitle]%
      \fi
    \else
      \newpage
      \global\@topnum\z@   % Prevents figures from going at top of page.
      \@maketitle
    \fi
    \thispagestyle{plain}\@thanks
  \endgroup
  \setcounter{footnote}{0}%
  \global\let\thanks\relax
  \global\let\maketitle\relax
  \global\let\@maketitle\relax
  \global\let\@thanks\@empty
  \global\let\@author\@empty
  \global\let\@date\@empty
  \global\let\@title\@empty
  \global\let\title\relax
  \global\let\author\relax
  \global\let\date\relax
  \global\let\and\relax
}
\def\@maketitle{%
  \newpage
  \null
  \vskip 2em%
  \begin{center}%
  \let \footnote \thanks
    {\LARGE \@title \par}%
    \vskip 1.5em%
    {\large
      \lineskip .5em%
      \begin{tabular}[t]{c}%
        \@author
      \end{tabular}\par}%
    \vskip 1em%
    {\large \@date}%
  \end{center}%
  \par
  \vskip 1.5em}
\fi
\newcommand*\chaptermark[1]{}
\setcounter{secnumdepth}{2}
\newcounter {part}
\newcounter {chapter}
\newcounter {section}[chapter]
\newcounter {subsection}[section]
\newcounter {subsubsection}[subsection]
\newcounter {paragraph}[subsubsection]
\newcounter {subparagraph}[paragraph]
\renewcommand \thepart {\@Roman\c@part}
\renewcommand \thechapter {\@arabic\c@chapter}
%
%----------------------------------------------------------
%% NOCHAP 07/24/06
\ifmultiplechapters
  \renewcommand \thesection {\thechapter.\@arabic\c@section}
\else
  \renewcommand \thesection {\@arabic\c@section}
\fi
%
%----------------------------------------------------------
\renewcommand\thesubsection   {\thesection.\@arabic\c@subsection}
\renewcommand\thesubsubsection{\thesubsection .\@arabic\c@subsubsection}
\renewcommand\theparagraph    {\thesubsubsection.\@arabic\c@paragraph}
\renewcommand\thesubparagraph {\theparagraph.\@arabic\c@subparagraph}
\newcommand\@chapapp{\chaptername}
\newcommand\part{%
  \if@openright
    \cleardoublepage
  \else
    \clearpage
  \fi
  \thispagestyle{plain}%
  \if@twocolumn
    \onecolumn
    \@tempswatrue
  \else
    \@tempswafalse
  \fi
  \null\vfil
  \secdef\@part\@spart}

\def\@part[#1]#2{%
    \ifnum \c@secnumdepth >-2\relax
      \refstepcounter{part}%
      \addcontentsline{toc}{part}{\thepart\hspace{1em}#1}%
    \else
      \addcontentsline{toc}{part}{#1}%
    \fi
    \markboth{}{}%
    {\centering
     \interlinepenalty \@M
     \normalfont
     \ifnum \c@secnumdepth >-2\relax
       \huge\bfseries \partname~\thepart
       \par
       \vskip 20\p@
     \fi
     \Huge \bfseries #2\par}%
    \@endpart}
\def\@spart#1{%
    {\centering
     \interlinepenalty \@M
     \normalfont
     \Huge \bfseries #1\par}%
    \@endpart}
\def\@endpart{\vfil\newpage
              \if@twoside
                \null
                \thispagestyle{empty}%
                \newpage
              \fi
              \if@tempswa
                \twocolumn
              \fi}
\newcommand\chapter{\if@openright\cleardoublepage\else\clearpage\fi
                    \thispagestyle{plain}%
                    \global\@topnum\z@
                    \@afterindentfalse
                    \secdef\@chapter\@schapter}
\def\@chapter[#1]#2{\ifnum \c@secnumdepth >\m@ne
                         \refstepcounter{chapter}%
                         \typeout{\@chapapp\space\thechapter.}%
                         \addcontentsline{toc}{chapter}%
                                   {\protect\numberline{\thechapter}#1}%
                    \else
                      \addcontentsline{toc}{chapter}{#1}%
                    \fi
                    \chaptermark{#1}%
%%---------------------------------------------------------------
%%---  SIU:  Changed to take out extra spacing                ---
%%---        in List of Tables and List of Figures            ---
%%---------------------------------------------------------------
%                    \addtocontents{lof}{\protect\addvspace{10\p@}}%
%                    \addtocontents{lot}{\protect\addvspace{10\p@}}%
                    \if@twocolumn
                      \@topnewpage[\@makechapterhead{#2}]%
                    \else
                      \@makechapterhead{#2}%
                      \@afterheading
                    \fi}
%%--------------------------------------------------------------
%%---  SIU:  (SIU-0): Change the top space from 50 to 30\p@  ---
%%---        (SIU-1): Change the \huge\bfseries (chapter     ---
%%---                 title) to \large\bfseries\centering.   ---
%%---        (SIU-2): Change vskip 20\p@ to vskip 1\p@.      ---
%%---        (SIU-2B): Change \Huge to \large and uppercase. ---
%%---        (SIU-3): Change vskip 40\p@ to vskip 24\p@.     ---
%%---        (SIU-4): Changed something...                   ---
%%---        (SIU-41): Changed vskip 30\p@ to no vskip       ---
%%---                  in chapter title, also took out       ---
%%---                  \large in chapter title.              ---
%%--------------------------------------------------------------
\def\@makechapterhead#1{%
%  \vspace*{30\p@}%          % (SIU-0) was 50
%  \vspace*{5\p@}%          % (SIU-4) was 30\p@
  {\parindent \z@ \raggedright \normalfont
    \ifnum \c@secnumdepth >\m@ne
       \bfseries\centering \@chapapp\space \thechapter %(SIU-41)
%        \large\bfseries\centering \@chapapp\space \thechapter %(SIU-1)
        \par\nobreak
        \vskip 1\p@   %(SIU-2)
    \fi
    \interlinepenalty\@M
%    \large\bfseries\centering\uppercase{#1}\par\nobreak  %(SIU-2B)
    \bfseries\centering\uppercase{#1}\par\nobreak  %(SIU-41)
    \vskip 24\p@      %(SIU-3)
  }}
\def\@schapter#1{\if@twocolumn
                   \@topnewpage[\@makeschapterhead{#1}]%
                 \else
                   \@makeschapterhead{#1}%
                   \@afterheading
                 \fi}
\def\@makeschapterhead#1{%
%  \vspace*{30\p@}%     % (SIU-0) was 50
  {\parindent \z@ \raggedright
    \normalfont
    \interlinepenalty\@M
%    \large\bfseries\centering \uppercase{#1}\par\nobreak  %(SIU-2B)
    \bfseries\centering \uppercase{#1}\par\nobreak  %(SIU-41)
    \vskip 24\p@     %(SIU-3)
  }}
%%--------------------------------------------------------------
%%---  SIU:  The following section turns on indents after    ---
%%---        headings.  Copied from the latex.ltx file.      ---
%%---        (and added the \@afterindentrue part.           ---
%%--------------------------------------------------------------
\def\@afterheading{%
  \@nobreaktrue
  \@afterindenttrue
  \everypar{%
    \if@nobreak
      \@nobreakfalse
      \clubpenalty \@M
      \if@afterindent \else
        {\setbox\z@\lastbox}%
      \fi
    \else
      \clubpenalty \@clubpenalty
      \everypar{}%
    \fi}}
%%--------------------------------------------------------------
%%---  SIU:  Change section's \Large to \large and           ---
%%---        subsection's \large to \normalsize.             ---
%%---        Changed all of the numbers below.               ---
%%--------------------------------------------------------------
%\newcommand\section{\@startsection {section}{1}{\z@}%
%                                   {-3.5ex \@plus -1ex \@minus -.2ex}%
%                                   {1.5ex \@plus.2ex}%  % was 2.3
%                                   {\normalfont\large\bfseries}}
\newcommand\section{\@startsection {section}{1}{\z@}%
%                                  {-3.0ex \@plus -1ex \@minus -.2ex}%
                                   {-3.5ex}%
                                   {0.1ex}%  % was 2.3
%                                  {\scshape\centering}}
            {\normalfont\normalsize\bfseries\MakeUppercase}}
%                                  {\normalfont\large\bfseries}}
\newcommand\subsection{\@startsection{subsection}{2}{\z@}%
%                                    {-3.0ex\@plus -1ex \@minus -.2ex}%
                                     {-3.5ex}%
                                     {0.1ex}%
                                     {\normalfont\normalsize\bfseries}}
\newcommand\subsubsection{\@startsection{subsubsection}{3}{\z@}%
%                                    {-3.0ex}%
                                     {-3.5ex}%
                                     {0.1ex}%
                                     {\normalfont\normalsize\bfseries}}
\newcommand\paragraph{\@startsection{paragraph}{4}{\z@}%
%                                   {3.0ex \@plus1ex \@minus.2ex}%
                                    {3.5ex}%
                                    {-1em}%
                                    {\normalfont\normalsize\bfseries}}
\newcommand\subparagraph{\@startsection{subparagraph}{5}{\parindent}%
%                                      {3.0ex}%
                                       {3.5ex}%
                                       {-1em}%
                                      {\normalfont\normalsize\bfseries}}
\if@twocolumn
  \setlength\leftmargini  {2em}
\else
  \setlength\leftmargini  {2.5em}
\fi
\leftmargin  \leftmargini
\setlength\leftmarginii  {2.2em}
\setlength\leftmarginiii {1.87em}
\setlength\leftmarginiv  {1.7em}
\if@twocolumn
  \setlength\leftmarginv  {.5em}
  \setlength\leftmarginvi {.5em}
\else
  \setlength\leftmarginv  {1em}
  \setlength\leftmarginvi {1em}
\fi
\setlength  \labelsep  {.5em}
\setlength  \labelwidth{\leftmargini}
\addtolength\labelwidth{-\labelsep}
\@beginparpenalty -\@lowpenalty
\@endparpenalty   -\@lowpenalty
\@itempenalty     -\@lowpenalty
\renewcommand\theenumi{\@arabic\c@enumi}
\renewcommand\theenumii{\@alph\c@enumii}
\renewcommand\theenumiii{\@roman\c@enumiii}
\renewcommand\theenumiv{\@Alph\c@enumiv}
\newcommand\labelenumi{\theenumi.}
\newcommand\labelenumii{(\theenumii)}
\newcommand\labelenumiii{\theenumiii.}
\newcommand\labelenumiv{\theenumiv.}
\renewcommand\p@enumii{\theenumi}
\renewcommand\p@enumiii{\theenumi(\theenumii)}
\renewcommand\p@enumiv{\p@enumiii\theenumiii}
\newcommand\labelitemi{\textbullet}
\newcommand\labelitemii{\normalfont\bfseries \textendash}
\newcommand\labelitemiii{\textasteriskcentered}
\newcommand\labelitemiv{\textperiodcentered}
\newenvironment{description}
               {\list{}{\labelwidth\z@ \itemindent-\leftmargin
                        \let\makelabel\descriptionlabel}}
               {\endlist}
\newcommand*\descriptionlabel[1]{\hspace\labelsep
                                \normalfont\bfseries #1}
\if@titlepage
  \newenvironment{abstract}{%
      \titlepage
      \null\vfil
      \@beginparpenalty\@lowpenalty
      \begin{center}%
%        \bfseries \abstractname	%BOLD
        \abstractname	%NO BOLD
        \@endparpenalty\@M
      \end{center}}%
     {\par\vfil\null\endtitlepage}
\else
  \newenvironment{abstract}{%
      \if@twocolumn
        \section*{\abstractname}%
      \else
        \small
        \begin{center}%
%          {\bfseries \abstractname\vspace{-.5em}\vspace{\z@}}%  BOLD
           \abstractname\vspace{-.5em}\vspace{\z@}}%  NO BOLD
        \end{center}%
        \quotation
      \fi}
      {\if@twocolumn\else\endquotation\fi}
\fi
\newenvironment{verse}
               {\let\\\@centercr
                \list{}{\itemsep      \z@
                        \itemindent   -1.5em%
                        \listparindent\itemindent
                        \rightmargin  \leftmargin
                        \advance\leftmargin 1.5em}%
                \item\relax}
               {\endlist}
\newenvironment{quotation}
               {\list{}{\listparindent 1.5em%
                        \itemindent    \listparindent
                        \rightmargin   \leftmargin
                        \parsep        \z@ \@plus\p@}%
                \item\relax}
               {\endlist}
\newenvironment{quote}
               {\list{}{\rightmargin\leftmargin}%
                \item\relax}
               {\endlist}
\if@compatibility
\newenvironment{titlepage}
    {%
      \if@twocolumn
        \@restonecoltrue\onecolumn
      \else
        \@restonecolfalse\newpage
      \fi
      \thispagestyle{empty}%
      \setcounter{page}\z@
    }%
    {\if@restonecol\twocolumn \else \newpage \fi
    }
\else
\newenvironment{titlepage}
    {%
      \if@twocolumn
        \@restonecoltrue\onecolumn
      \else
        \@restonecolfalse\newpage
      \fi
      \thispagestyle{empty}%
      \setcounter{page}\@ne
    }%
    {\if@restonecol\twocolumn \else \newpage \fi
     \if@twoside\else
        \setcounter{page}\@ne
     \fi
    }
\fi
\newcommand\appendix{\par
  \setcounter{chapter}{0}%
  \setcounter{section}{0}%
  \gdef\@chapapp{\appendixname}%
  \gdef\thechapter{\@Alph\c@chapter}}
\setlength\arraycolsep{5\p@}
\setlength\tabcolsep{6\p@}
\setlength\arrayrulewidth{.4\p@}
\setlength\doublerulesep{2\p@}
\setlength\tabbingsep{\labelsep}
\skip\@mpfootins = \skip\footins
\setlength\fboxsep{3\p@}
\setlength\fboxrule{.4\p@}
\@addtoreset {equation}{chapter}
%
%----------------------------------------------------------
%% NOCHAP 07/24/06
\ifmultiplechapters
   \renewcommand\theequation
     {\ifnum \c@chapter>\z@ \thechapter.\fi \@arabic\c@equation}
   \newcounter{figure}[chapter]
   \renewcommand \thefigure
        {\ifnum \c@chapter>\z@ \thechapter.\fi \@arabic\c@figure}
\else
    \renewcommand\theequation
      {\@arabic\c@equation}
    \newcounter{figure}
    \renewcommand \thefigure
         {\@arabic\c@figure}
\fi
%----------------------------------------------------------
\def\fps@figure{tbp}
\def\ftype@figure{1}
\def\ext@figure{lof}
\def\fnum@figure{\figurename~\thefigure}
\newenvironment{figure}
               {\@float{figure}}
               {\end@float}
\newenvironment{figure*}
               {\@dblfloat{figure}}
               {\end@dblfloat}
%
%----------------------------------------------------------
%% NOCHAP 07/24/06
\ifmultiplechapters
   \newcounter{table}[chapter]
   \renewcommand \thetable
        {\ifnum \c@chapter>\z@ \thechapter.\fi \@arabic\c@table}
\else
   \newcounter{table}
   \renewcommand \thetable
        {\@arabic\c@table}
\fi
%----------------------------------------------------------
%
\def\fps@table{tbp}
\def\ftype@table{2}
\def\ext@table{lot}
\def\fnum@table{\tablename~\thetable}
\newenvironment{table}
               {\@float{table}}
               {\end@float}
\newenvironment{table*}
               {\@dblfloat{table}}
               {\end@dblfloat}
%%--------------------------------------------------------------
%%---  SIU:  Reset the abovecaptionskip from 10\p@ to 14\p@  ---
%%--------------------------------------------------------------
\newlength\abovecaptionskip
\newlength\belowcaptionskip
\setlength\abovecaptionskip{14\p@}
\setlength\belowcaptionskip{0\p@}
%%--------------------------------------------------------------
%%---  SIU:  Change the #1: #2 below to #1. #2 (what appears ---
%%---        between the figure number and the caption.      ---
%%---        Also go to single spacing and indent (a).       ---
%%--------------------------------------------------------------
\long\def\@makecaption#1#2{%
  \vskip\abovecaptionskip
%  \sbox\@tempboxa{{\scshape #1}. #2}%
  \sbox\@tempboxa{{#1}. #2}%
  \ifdim \wd\@tempboxa >\hsize
  {\baselineskip=14truept      % <- (a)
   \leftskip=0.7truein         % <- (a)
   \rightskip=0.7truein        % <- (a)
%   {\scshape #1}. #2\par                 % <- (a)
    {#1}. #2\par                 % <- (a)
   }                           % <- (a)
  \else
    \global \@minipagefalse
    \hb@xt@\hsize{\hfil\box\@tempboxa\hfil}%
  \fi
  \vskip\belowcaptionskip}
\DeclareOldFontCommand{\rm}{\normalfont\rmfamily}{\mathrm}
\DeclareOldFontCommand{\sf}{\normalfont\sffamily}{\mathsf}
\DeclareOldFontCommand{\tt}{\normalfont\ttfamily}{\mathtt}
\DeclareOldFontCommand{\bf}{\normalfont\bfseries}{\mathbf}
\DeclareOldFontCommand{\it}{\normalfont\itshape}{\mathit}
\DeclareOldFontCommand{\sl}{\normalfont\slshape}{\@nomath\sl}
\DeclareOldFontCommand{\sc}{\normalfont\scshape}{\@nomath\sc}
\DeclareRobustCommand*\cal{\@fontswitch\relax\mathcal}
\DeclareRobustCommand*\mit{\@fontswitch\relax\mathnormal}
%%--------------------------------------------------------------
%%---  SIU:  Changed pnumwidth below from 1.55em (for TOC)   ---
%%--------------------------------------------------------------
\newcommand\@pnumwidth{2.00em}
\newcommand\@tocrmarg{2.55em}
\newcommand\@dotsep{4.5}
\setcounter{tocdepth}{2}
\newcommand\tableofcontents{%
    \if@twocolumn
      \@restonecoltrue\onecolumn
    \else
      \@restonecolfalse
    \fi
    \chapter*{\contentsname
        \@mkboth{%
           \MakeUppercase\contentsname}{\MakeUppercase\contentsname}}%
    \@starttoc{toc}%
    \if@restonecol\twocolumn\fi
    }
\newcommand*\l@part[2]{%
  \ifnum \c@tocdepth >-2\relax
    \addpenalty{-\@highpenalty}%
%%---------------------------------------------------------------
%%---  SIU:  Changed to take out extra spacing                ---
%%---------------------------------------------------------------
%    \addvspace{2.25em \@plus\p@}%
    \begingroup
      \parindent \z@ \rightskip \@pnumwidth
      \parfillskip -\@pnumwidth
      {\leavevmode
       \large \bfseries #1\hfil \hb@xt@\@pnumwidth{\hss #2}}\par
       \nobreak
         \global\@nobreaktrue
         \everypar{\global\@nobreakfalse\everypar{}}%
    \endgroup
  \fi}
%%--------------------------------------------------------------
%%---  SIU:  Modified the \l@chapter section below to add    ---
%%---        the dotted line and to not skip so much before. ---
%%---        Also took out bolding and extra space on        ---
%%---        \chapter lines in toc.                          ---
%%--------------------------------------------------------------
\newcommand*\l@chapter[2]{%
  \ifnum \c@tocdepth >\m@ne
    \addpenalty{-\@highpenalty}%
  % \vskip 1.0em \@plus\p@
%    \vskip 3pt
    \setlength\@tempdima{1.5em}%
    \begingroup
      \parindent \z@ \rightskip \@pnumwidth
      \parfillskip -\@pnumwidth
      \leavevmode %\bfseries
      \advance\leftskip\@tempdima
      \hskip -\leftskip
      #1\nobreak %\hfil
%%--------------------------------------------------------------
%%---  SIU:  The following three lines were copied from the  ---
%%---        dottedtocline definition in latex.ltx, adding   ---
%%---        the normalfont so that the .'s are not bolded.  ---
%%--------------------------------------------------------------
     \leaders\hbox{$\m@th
        \mkern \@dotsep mu\hbox{\normalfont .}\mkern \@dotsep
        mu$}\hfill
      \nobreak\hb@xt@\@pnumwidth{\hss #2}\par
      \penalty\@highpenalty
    \endgroup
  \fi}
%%--------------------------------------------------------------
%%---  SIU:  And... if you comment out all of the l@chapter  ---
%%---        above and replace with the following line, then ---
%%---        the chapter headings aren't bolded at all.      ---
%%--------------------------------------------------------------
%\newcommand*\l@chapter{\vskip 10pt\@dottedtocline{0}{1.5em}{2.3em}}
\newcommand*\l@section{\@dottedtocline{1}{1.5em}{2.3em}}
\newcommand*\l@subsection{\@dottedtocline{2}{3.8em}{3.2em}}
\newcommand*\l@subsubsection{\@dottedtocline{3}{7.0em}{4.1em}}
\newcommand*\l@paragraph{\@dottedtocline{4}{10em}{5em}}
\newcommand*\l@subparagraph{\@dottedtocline{5}{12em}{6em}}
\newcommand\listoffigures{%
    \if@twocolumn
      \@restonecoltrue\onecolumn
    \else
      \@restonecolfalse
    \fi
    \chapter*{\MakeUppercase\listfigurename
      \@mkboth{\MakeUppercase\listfigurename}%
              {\MakeUppercase\listfigurename}}%
    \addcontentsline{toc}{chapter}{\listfigurename}  % SIU added line
    \@starttoc{lof}%
    \if@restonecol\twocolumn\fi
    }
\newcommand*\l@figure{\@dottedtocline{0}{0.0em}{2.3em}}  % SIU {1}to{0}
\newcommand\listoftables{%
    \if@twocolumn
      \@restonecoltrue\onecolumn
    \else
      \@restonecolfalse
    \fi
    \chapter*{\MakeUppercase\listtablename
      \@mkboth{%
          \MakeUppercase\listtablename}{\MakeUppercase\listtablename}}%
    \addcontentsline{toc}{chapter}{\listtablename}  % SIU added line
    \@starttoc{lot}%
    \if@restonecol\twocolumn\fi
    }
\let\l@table\l@figure
\newdimen\bibindent
\setlength\bibindent{1.5em}
%%--------------------------------------------------------------
%%---  SIU:  Add baselineskip command to force single space. ---
%%---        Put in footnotesize                             ---
%%--------------------------------------------------------------
\newenvironment{thebibliography}[1]
     {\chapter*{\bibname
        \@mkboth{\MakeUppercase\bibname}{\MakeUppercase\bibname}}%
      \list{\@biblabel{\@arabic\c@enumiv}}%
           {\settowidth\labelwidth{\@biblabel{#1}}%
            \leftmargin\labelwidth
%     \footnotesize           % <-- SIU
%     \baselineskip=14truept  % <-- SIU
      \setlength{\itemsep}{0pt}  % <-- SIU
      \setlength{\parsep}{0pt}   % <-- SIU
            \advance\leftmargin\labelsep
            \@openbib@code
            \usecounter{enumiv}%
            \let\p@enumiv\@empty
            \renewcommand\theenumiv{\@arabic\c@enumiv}}%
      \sloppy
      \clubpenalty4000
      \@clubpenalty \clubpenalty
      \widowpenalty4000%
      \sfcode`\.\@m}
     {\def\@noitemerr
       {\@latex@warning{Empty `thebibliography' environment}}%
      \endlist}
\newcommand\newblock{\hskip .11em\@plus.33em\@minus.07em}
\let\@openbib@code\@empty
\newenvironment{theindex}
               {\if@twocolumn
                  \@restonecolfalse
                \else
                  \@restonecoltrue
                \fi
                \columnseprule \z@
                \columnsep 35\p@
                \twocolumn[\@makeschapterhead{\indexname}]%
                \@mkboth{\MakeUppercase\indexname}%
                        {\MakeUppercase\indexname}%
                \thispagestyle{plain}\parindent\z@
                \parskip\z@ \@plus .3\p@\relax
                \let\item\@idxitem}
               {\if@restonecol\onecolumn\else\clearpage\fi}
\newcommand\@idxitem{\par\hangindent 40\p@}
\newcommand\subitem{\@idxitem \hspace*{20\p@}}
\newcommand\subsubitem{\@idxitem \hspace*{30\p@}}
\newcommand\indexspace{\par \vskip 10\p@ \@plus5\p@ \@minus3\p@\relax}
\renewcommand\footnoterule{%
  \kern-3\p@
  \hrule\@width.4\columnwidth
  \kern2.6\p@}
\@addtoreset{footnote}{chapter}
\newcommand\@makefntext[1]{%
    \parindent 1em%
    \noindent
    \hb@xt@1.8em{\hss\@makefnmark}#1}
%%--------------------------------------------------------------
%%---  SIU:  Not sure of the best place to put it, but set   ---
%%---        all of the page margins, etc... here.           ---
%%--------------------------------------------------------------
\pagestyle{plain}
\tolerance=10000
\hbadness=10000
\raggedbottom
\setlength{\footskip}{30pt}
\setlength{\textwidth}{6.4in}   %% SIU-50 was 5.9 in
\setlength{\textheight}{8.9in}
\setlength{\topmargin}{-0.40in}
\setlength{\oddsidemargin}{0.05in}  %% SIU-50 was 0.55
\setlength{\evensidemargin}{0.05in}  %% SIU-50 was 0.55
\setlength{\parindent}{.4in}
\setlength{\abovedisplayskip}{12pt plus0pt minus0pt}
\setlength{\belowdisplayskip}{12pt plus0pt minus0pt}
\setlength{\abovedisplayshortskip}{2pt plus0pt minus0pt}
\setlength{\belowdisplayshortskip}{12pt plus0pt minus0pt}
\setlength{\intextsep}{6pt plus 0pt minus 0pt}
\providecommand{\bysame}{\makebox[3em]{\hrulefill}\thinspace}
%%--------------------------------------------------------------
%%---  SIU:  Change CONTENTS, LIST OF FIGURES, LIST OF       ---
%%---        TABLES, BIBLIOGRAPHY, INDEX, CHAPTER, APPENDIX, ---
%%---        and ABSTRACT in following names to Uppercase.   ---
%%--------------------------------------------------------------
\newcommand\contentsname{TABLE OF CONTENTS}
\newcommand\listfigurename{List of Figures}
\newcommand\listtablename{List of Tables}
\newcommand\bibname{REFERENCES}
\newcommand\indexname{INDEX}
\newcommand\figurename{Figure}
\newcommand\tablename{Table}
\newcommand\partname{Part}
\newcommand\chaptername{CHAPTER}
\newcommand\appendixname{APPENDIX}
\newcommand\abstractname{ABSTRACT}
\def\today{\ifcase\month\or
  January\or February\or March\or April\or May\or June\or
  July\or August\or September\or October\or November\or December\fi
  \space\number\day, \number\year}
\setlength\columnsep{10\p@}
\setlength\columnseprule{0\p@}
\pagestyle{plain}
\pagenumbering{arabic}
\if@twoside
\else
  \raggedbottom
\fi
\if@twocolumn
  \twocolumn
  \sloppy
  \flushbottom
\else
  \onecolumn
\fi
\endinput
%%
%% End of file `report.cls'.
