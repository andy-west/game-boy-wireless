using System;
using System.Collections.Generic;
using System.Linq;

namespace GameBoyServer
{
    public class Board
    {
        public int[,] Squares { get; }
        public PlaceDiscResult LastMove { get; private set; }

        public Board()
        {
            Squares = new int[8,8];
            Reset();
        }

        public void Reset()
        {
            for (var y = 0; y < 8; y++)
            {
                for (var x = 0; x < 8; x++)
                {
                    Squares[x, y] = 0;
                }
            }

            Squares[3, 3] = 2;
            Squares[4, 3] = 1;
            Squares[3, 4] = 1;
            Squares[4, 4] = 2;

            LastMove = new PlaceDiscResult(PlayerColor.None, -1, -1, false, null);
        }

        public PlaceDiscResult TryPlaceDisc(PlayerColor playerColor, int x, int y)
        {
            bool isMoveValid;
            var discsToFlip = new List<int[]>();

            if (!AreCoordinatesInBounds(x, y) || Squares[x, y] != 0)
            {
                isMoveValid = false;
            }
            else
            {
                var otherPlayerColor = playerColor == PlayerColor.Black ? PlayerColor.White : PlayerColor.Black;

                CheckDirection(playerColor, otherPlayerColor, x, y, 0, -1, discsToFlip);   // Up
                CheckDirection(playerColor, otherPlayerColor, x, y, 1, -1, discsToFlip);   // Up-right
                CheckDirection(playerColor, otherPlayerColor, x, y, 1, 0, discsToFlip);    // Right
                CheckDirection(playerColor, otherPlayerColor, x, y, 1, 1, discsToFlip);    // Down-right
                CheckDirection(playerColor, otherPlayerColor, x, y, 0, 1, discsToFlip);    // Down
                CheckDirection(playerColor, otherPlayerColor, x, y, -1, 1, discsToFlip);   // Down-left
                CheckDirection(playerColor, otherPlayerColor, x, y, -1, 0, discsToFlip);   // Left
                CheckDirection(playerColor, otherPlayerColor, x, y, -1, -1, discsToFlip);  // Up-left

                isMoveValid = discsToFlip.Any();
            }

            if (isMoveValid)
            {
                Squares[x, y] = (int)playerColor;

                foreach (var discToFlip in discsToFlip)
                {
                    Squares[discToFlip[0], discToFlip[1]] = (int)playerColor;
                }
            }

            var result = new PlaceDiscResult(playerColor, x, y, isMoveValid, discsToFlip.ToArray());

            if (isMoveValid)
            {
                LastMove = result;
            }

            return result;
        }

        public bool AreCoordinatesInBounds(int x, int y)
        {
            return x >= 0 && x < 8 && y >= 0 && y < 8;
        }

        public void CheckDirection(PlayerColor playerColor, PlayerColor otherPlayerColor,
            int x, int y, int xDirection, int yDirection, List<int[]> discsToFlip)
        {
            if (xDirection == -1 && x < 2
                || xDirection == 1 && x > 5
                || yDirection == -1 && y < 2
                || yDirection == 1 && y > 5
                || Squares[x + xDirection, y + yDirection] != (int)otherPlayerColor)
            {
                return;
            }

            var x2 = x + xDirection * 2;
            var y2 = y + yDirection * 2;

            var playerColorFound = false;

            while (AreCoordinatesInBounds(x2, y2))
            {
                var square = Squares[x2, y2];

                if (square == 0)
                {
                    break;
                }

                if (square == (int)playerColor)
                {
                    playerColorFound = true;
                    break;
                }

                x2 += xDirection;
                y2 += yDirection;
            }

            if (playerColorFound)
            {
                var x3 = x + xDirection;
                var y3 = y + yDirection;

                var discCount = Math.Max(Math.Abs(x2 - x), Math.Abs(y2 - y)) - 1;

                for (var i = 0; i < discCount ; i++)
                {
                    discsToFlip.Add(new[] { x3, y3 });

                    x3 += xDirection;
                    y3 += yDirection;
                }
            }
        }
    }
}
