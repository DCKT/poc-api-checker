type book = {
  id: string,
  title: string,
  description: string,
  price: float,
  tags: array(string),
};

type books = array(book);