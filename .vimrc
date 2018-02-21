set tabstop=4       " The width of a TAB is set to 4.
                    " Still it is a \t. It is just that
                    " Vim will interpret it to be having
                    " a width of 4.

set shiftwidth=4    " Indents will have a width of 4

set softtabstop=4   " Sets the number of columns for a TAB

set expandtab       " Expand TABs to spaces

set autoindent      " Copies indent of previous line

set copyindent      " ^^^^^

set relativenumber          " Displays line numbers on startup

"highlight OverLength ctermbg=red ctermfg=white guibg=#592929
"match OverLength /\%81v.\+/

inoremap jk <ESC>
nnoremap <space> d$
nnoremap L $
inoremap <ESC> <nop>
nnoremap <BACKSPACE> cb
" set spell spelllang=en_us

syntax on

set textwidth=150

set smartindent 

set smarttab

set backspace=indent,eol,start
set ruler

call plug#begin('~/.vim/plugged')

Plug 'lervag/vimtex'

call plug#end()
let g:tex_flavor='latex'

set nocompatible
filetype off

filetype plugin indent on

" Settings
"set grepprg=grep\ -nH\ $*
"set sw=2
"set iskeyword+=:
"let g:tex_flavor='latex'
"let g:tex_fold_enabled=1
"let g:tex_nospell=1
"let g:tex_no_error=1
"
"let g:vimtex_view_general_viewer
"      \ = '/Applications/Skim.app/Contents/SharedSupport/displayline'
"let g:vimtex_view_general_options = '-r @line @pdf @tex'
"let g:vimtex_latexmk_callback_hooks = ['UpdateSkim']
"
"function! UpdateSkim(status)
"  if !a:status | return | endif
"
"  let l:out = b:vimtex.out()
"  let l:tex = expand('%:p')
"  let l:cmd = [g:vimtex_view_general_viewer, '-r']
"  if !empty(system('pgrep Skim'))
"    call extend(l:cmd, ['-g'])
"  endif
"  if has('nvim')
"    call jobstart(l:cmd + [line('.'), l:out, l:tex])
"  elseif has('job')
"    call job_start(l:cmd + [line('.'), l:out, l:tex])
"  else
"    call system(join(l:cmd + [line('.'),
"          \ shellescape(l:out), shellescape(l:tex)], ' '))
"  endif
"endfunction
