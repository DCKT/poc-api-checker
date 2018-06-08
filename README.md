# POC Type check your API

My case is : I want to make it sure that my payload types I wrote are matching with the current API I'm working with. Let's create a fake book API with `json-server` :

```json
{
  "books": [
    {
      "id": "1",
      "title": "Hello world",
      "description": "A sweet introduction",
      "price": 10.0,
      "tags": ["smart", "awesome"]
    }
  ]
}
```

My favorite HTTP client is Axios, so let's make a request for a book :

```
Axios.get("http://localhost:3000/books/1")
|> Js.Promise.then_(response => {
  let book = response##data;
  Js.log("I assume there is a price");
  Js.log(book##price);
  Js.Promise.resolve();
});
```

This is not good, we are not safe with the data, the response type is too abstract and we don't get any feeback from the compiler, let's force this with a `book` type :

```ocaml
type book = {
  id: string,
  title: string,
  description: string,
  price: float,
  tags: array(string),
};
```

This is better BUT after your holidays, the Project Owner decided to change the payload of this endpoint but forget to told, until when you will know how fucked-up is your code ? At runtime 🙃

There is no way to prevent runtime error like this, the only thing you can do is **reducing** the time spent for debugging. Before running your project, run some tests that will check every endpoint of your app are correctly responding the good payload.

### How test our type on runtime ?

With the help of [bs-json](https://github.com/glennsl/bs-json) package, we will take advantage of JSON encode/decode system.
`bs-json` allow us to declare a function which will parse and check every field, so if the parsing fail it's because something change, let's create a decoder for our `book` type :

```ocaml
module Decode = {
  let book: Js.Json.t => book =
    json =>
      Json.Decode.{
        id: json |> field("id", string),
        title: json |> field("title", string),
        description: json |> field("description", string),
        price: json |> field("price", float),
        tags: json |> field("tags", array(string)),
      };
};
```

Now, the `book` type mirror the decode book function so, if we change something in the type and forget to edit it somewhere, the compiler will warn us 💪.

### Let's test

To avoid too much boilerplate, I create a small function that will handle the request (only Axios btw), which take 3 params :

* label : a string to help us debugging
* request : the Promise request
* decoder : the `bs-json` decoder which will be run

```ocaml
ApiChecker.check(
  ~label="Book details",
  ~request=Axios.get("http://localhost:3000/books/1"),
  ~decoder=Decode.book,
);
```

If we run the code, nothing should be output, but if we change the payload like transforming `title` into an `int` :

```console
 ===> Endpoint Book details
[ [ 'Json_decode.DecodeError', 4, tag: 248 ],
  'Expected string, got 1' ]
```

This is pretty cool right ? The only things, is about the stacktrace which is pretty ugly but good enough for our case 😏.

## Run the example

* Run the fake server : `npm run api`
* Run the test command : `npm t`

## Contributing

* `npm i`
* `npm start`
