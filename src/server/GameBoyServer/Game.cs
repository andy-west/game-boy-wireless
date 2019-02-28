using System.Collections.Generic;

namespace GameBoyServer
{
    public class Game : IGame
    {
        public GameState State { get; set; }
        public List<Player> Players { get; }
        public Board Board { get; }

        public Game()
        {
            Players = new List<Player>();
            Board = new Board();
        }

        public void Reset()
        {
            State = GameState.WaitingForPlayers;
            Players.Clear();
            Board.Reset();
        }
    }
}
