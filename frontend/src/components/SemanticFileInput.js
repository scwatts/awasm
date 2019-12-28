import React from 'react';
import { Button, Input } from 'semantic-ui-react';


class SemanticFileInput extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      filename: null
    };
    this.handleFileChange = this.handleFileChange.bind(this)
  }

  fileInputRef = React.createRef();

  handleFileChange(event) {
    this.setState({ filename: event.target.files[0].name })
  }

  render() {
    const { button_label, idname } = this.props;

    return(
      <div>
        <Input disabled value={this.state.filename} />
        <Button
          content={button_label}
          labelPosition="left"
          icon="file"
          onClick={() => this.fileInputRef.current.click()}
        />
        <input
          id={idname}
          ref={this.fileInputRef}
          type="file"
          hidden
          onChange={this.handleFileChange}
        />
      </div>
    );
  }
}


export default SemanticFileInput;
