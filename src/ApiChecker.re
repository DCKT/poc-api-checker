type book = {
  id: string,
  title: string,
  description: string,
  price: float,
  tags: array(string),
};

type books = array(book);

module Decode = {
  let book = json =>
    Json.Decode.{
      id: json |> field("id", string),
      title: json |> field("title", string),
      description: json |> field("description", string),
      price: json |> field("price", float),
      tags: json |> field("tags", array(string)),
    };
  let books = json => Json.Decode.array(book);
};

let data = {|
  {
    "id": "id-1",
    "title": "Greetings.",
    "description": "A welcoming introduction",
    "price": 10.0,
    "tags": ["smart", "awesome"]
  }
|};

let config = Axios.makeConfig(~baseURL="http://localhost:3000", ());

let axiosInstance = Axios.Instance.create(config);

let checkStruct = (data, decoder) =>
  data |> Js.Json.stringify |> Json.parseOrRaise |> decoder |> ignore;

let handler = (name, promise, decoder) =>
  promise
  |> Js.Promise.then_(response => {
       checkStruct(response##data, decoder);
       Js.Promise.resolve();
     })
  |> Js.Promise.catch(error => {
       Js.log("Endpoint " ++ name);
       Js.log(error);
       Js.Promise.resolve();
     })
  |> ignore;

handler(
  "Book details",
  Axios.Instance.get(axiosInstance, "/books/1"),
  Decode.book,
);

handler(
  "Books root",
  Axios.Instance.get(axiosInstance, "/books"),
  Decode.books,
);