import chess.pgn

def pgn_to_uci(input_pgn_file, output_file):
    with open(input_pgn_file, "r") as pgn_file, open(output_file, "w") as output:
        while True:
            game = chess.pgn.read_game(pgn_file)
            if game is None:
                break  # No more games in the PGN file
            
            board = game.board()
            moves = []
            
            for move in game.mainline_moves():
                uci_move = move.uci()
                moves.append(uci_move)
                board.push(move)
            
            # Write the move sequence as a single line
            output.write(" ".join(moves) + "\n")

# Example usage:
pgn_to_uci("8moves_v3.pgn", "output_opening_book.txt")
