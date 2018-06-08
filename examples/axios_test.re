module Decode = {
  let book: Js.Json.t => SharedTypes.book =
    json =>
      Json.Decode.{
        id: json |> field("id", string),
        title: json |> field("title", string),
        description: json |> field("description", string),
        price: json |> field("price", float),
        tags: json |> field("tags", array(string)),
      };
  let books = _json => Json.Decode.array(book);
};

let config = Axios.makeConfig(~baseURL="http://localhost:3000", ());

let axiosInstance = Axios.Instance.create(config);

ApiChecker.check(
  ~label="Book details",
  ~request=Axios.Instance.get(axiosInstance, "/books/1"),
  ~decoder=Decode.book,
);

ApiChecker.check(
  ~label="Books root",
  ~request=Axios.Instance.get(axiosInstance, "/books"),
  ~decoder=Decode.books,
);