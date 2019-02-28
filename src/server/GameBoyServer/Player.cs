namespace GameBoyServer
{
    public class Player
    {
        public int GameBoyId { get; }
        public PlayerColor Color { get; }

        public Player(int gameBoyId, PlayerColor color)
        {
            GameBoyId = gameBoyId;
            Color = color;
        }
    }
}
