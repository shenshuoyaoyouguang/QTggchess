TEMPLATE = subdirs
# ponytail: tst_polyglotbook is excluded. It uses OpeningBook::Ram/Disk
# and Board::StandardAlgebraic, both western-chess APIs removed in
# e01b34a ("Clean up unused engines, components, and variants"). The
# test file was not updated for the Xiangqi fork, so it no longer
# compiles. Re-add when/if a Chinese-chess Polyglot book adapter is
# written.
SUBDIRS = chessboard eco sprt tournamentplayer tournamentpair
